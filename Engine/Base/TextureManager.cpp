#include "TextureManager.h"
#include <cassert>

//実体定義
TextureManager* TextureManager::instance = nullptr;
uint32_t TextureManager::descriptorSizeSRV = 0;


TextureManager* TextureManager::GetInstance() {
	if (instance == nullptr) {
		instance = new TextureManager();
	}
	return instance;
}


void TextureManager::DeleteInstnace() {
	delete instance;
	instance = nullptr;
}


uint32_t TextureManager::Load(const std::string& filePath) {
	//テクスチャを読み込む
	uint32_t textureHandle = TextureManager::GetInstance()->LoadInternal(filePath);

	return textureHandle;
}


void TextureManager::Initialize() {
	//デバイスの取得
	device_ = FCS::GetInstance()->GetDevice();
	//コマンドリストの取得
	commandList_ = FCS::GetInstance()->GetCommandList();

	//インクリメントサイズの初期化
	TextureManager::descriptorSizeSRV = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//ディスクリプタヒープの作成
	srvDescriptorHeap_ = FCS::GetInstance()->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kNumDescriptors, true);

	//デフォルト画像を読み込む
	LoadInternal("Resources/white.png");
}


void TextureManager::SetGraphicsDescriptorHeap() {
	//DescriptorHeapを設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { srvDescriptorHeap_.Get() };
	commandList_->SetDescriptorHeaps(1, descriptorHeaps);
}


void TextureManager::SetGraphicsRootDescriptorTable(UINT rootParameterIndex, uint32_t textureHandle) {
	//SRVのDescriptorTableの先頭を設定
	commandList_->SetGraphicsRootDescriptorTable(rootParameterIndex, textures_[textureHandle].srvHandleGPU);
}


const D3D12_RESOURCE_DESC TextureManager::GetResourceDesc(uint32_t textureHandle) {
	//テクスチャの情報を取得
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc = textures_[textureHandle].resource->GetDesc();

	return resourceDesc;
}


uint32_t TextureManager::CreateInstancingShaderResourceView(const Microsoft::WRL::ComPtr<ID3D12Resource>& instancingResource, uint32_t kNumInstance, size_t size) {
	textureHandle_++;
	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements = kNumInstance;
	instancingSrvDesc.Buffer.StructureByteStride = UINT(size);
	textures_[textureHandle_].srvHandleCPU = GetCPUDescriptorHandle(srvDescriptorHeap_.Get(), descriptorSizeSRV, textureHandle_);
	textures_[textureHandle_].srvHandleGPU = GetGPUDescriptorHandle(srvDescriptorHeap_.Get(), descriptorSizeSRV, textureHandle_);
	device_->CreateShaderResourceView(instancingResource.Get(), &instancingSrvDesc, textures_[textureHandle_].srvHandleCPU);
	return textureHandle_;
}


uint32_t TextureManager::LoadInternal(const std::string& filePath) {
	//同じテクスチャがないか探す
	for (int i = 0; i < kNumDescriptors; i++) {
		//同じテクスチャがあった場合そのテクスチャハンドルを返す
		if (textures_[i].name == filePath) {
			return textures_[i].textureHandle;
		}
	}

	//テクスチャハンドルをインクリメント
	textureHandle_++;
	//テクスチャがディスクリプタの最大数を超えていたら止める
	if (textureHandle_ >= kNumDescriptors) {
		assert(0);
	}

	//テクスチャを読み込む
	DirectX::ScratchImage mipImages = TextureManager::LoadTexture(filePath);

	//メタデータを取得
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	//テクスチャ情報を基にリソースを作成
	textures_[textureHandle_].resource = TextureManager::CreateTextureResource(metadata);
	//テクスチャのリソースにデータを転送する
	textures_[textureHandle_].intermediateResource = TextureManager::UploadTextureData(textures_[textureHandle_].resource.Get(), mipImages);

	//metaDataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

	//SRVを作成するDescriptorHeapの場所を決める
	textures_[textureHandle_].srvHandleCPU = TextureManager::GetCPUDescriptorHandle(srvDescriptorHeap_.Get(), descriptorSizeSRV, textureHandle_);
	textures_[textureHandle_].srvHandleGPU = TextureManager::GetGPUDescriptorHandle(srvDescriptorHeap_.Get(), descriptorSizeSRV, textureHandle_);

	//SRVの作成
	device_->CreateShaderResourceView(textures_[textureHandle_].resource.Get(), &srvDesc, textures_[textureHandle_].srvHandleCPU);

	//テクスチャの名前を保存する
	textures_[textureHandle_].name = filePath;
	//テクスチャハンドルを保存する
	textures_[textureHandle_].textureHandle = textureHandle_;


	return textureHandle_;
}


DirectX::ScratchImage TextureManager::LoadTexture(const std::string& filePath) {
	//テクスチャファイルを読んでプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	//ミップマップの作成
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

	//ミップマップ付きのデータを返す
	return mipImages;
}


Microsoft::WRL::ComPtr<ID3D12Resource> TextureManager::CreateTextureResource(const DirectX::TexMetadata& metadata) {
	//metadataを基にResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(metadata.width);//Textureの幅
	resourceDesc.Height = UINT(metadata.height);//Textureの高さ
	resourceDesc.MipLevels = UINT16(metadata.mipLevels);//mipmapの数
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);//奥行or配列Textureの配列数
	resourceDesc.Format = metadata.format;//TextureのFormat
	resourceDesc.SampleDesc.Count = 1;//サンプルカウント。1固定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);//Textureの次元数。普段使っているのは2次元

	//利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	//Resourceの作成
	Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;
	HRESULT hr = device_->CreateCommittedResource(
		&heapProperties,//Heapの設定
		D3D12_HEAP_FLAG_NONE,//Heapの特殊な設定。特になし。
		&resourceDesc,//Resourceの設定
		D3D12_RESOURCE_STATE_COPY_DEST,//データ転送される設定
		nullptr,//Clear最適値。使わないのでnullptr
		IID_PPV_ARGS(&resource));//作成するResourceのポインタ
	assert(SUCCEEDED(hr));

	return resource;
}


[[nodiscard]]
Microsoft::WRL::ComPtr<ID3D12Resource> TextureManager::UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages) {
	//中間リソースを作成
	std::vector<D3D12_SUBRESOURCE_DATA> subresources;
	DirectX::PrepareUpload(device_, mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subresources);
	uint64_t intermediateSize = GetRequiredIntermediateSize(texture, 0, UINT(subresources.size()));
	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource = FCS::GetInstance()->CreateBufferResource(intermediateSize);

	//データ転送をコマンドに積む
	UpdateSubresources(commandList_, texture, intermediateResource.Get(), 0, 0, UINT(subresources.size()), subresources.data());

	//Textureへの転送後は利用できるよう、D3D12_RESOURCE_STATE_COPY_DESTからD3D12_RESOURCE_STATE_GENERIC_READへResourceStateを変更する
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = texture;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	commandList_->ResourceBarrier(1, &barrier);

	return intermediateResource;
}


D3D12_CPU_DESCRIPTOR_HANDLE TextureManager::GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, const uint32_t descriptorSize, uint32_t index) {
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += static_cast<D3D12_CPU_DESCRIPTOR_HANDLE>((descriptorSize * index)).ptr;
	return handleCPU;
}


D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, const uint32_t descriptorSize, uint32_t index) {
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += static_cast<D3D12_GPU_DESCRIPTOR_HANDLE>((descriptorSize * index)).ptr;
	return handleGPU;
}