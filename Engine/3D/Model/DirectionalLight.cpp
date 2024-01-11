#include "DirectionalLight.h"
#include "Engine/Base/FireControlSystem.h"

void DirectionalLight::Initialize() {
	//lightingResourceの作成
	lightingResource_ = FCS::GetInstance()->CreateBufferResource(sizeof(ConstBufferDataDirectionalLight));

	//lightingResourceに書き込む
	ConstBufferDataDirectionalLight* directionalLightData = nullptr;
	lightingResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));
	directionalLightData->lightingMethod = lightingMethod_;
	directionalLightData->enableLighting = enableLighting_;
	directionalLightData->color = color_;
	directionalLightData->direction = direction_;
	directionalLightData->intensity = intensity_;
	lightingResource_->Unmap(0, nullptr);
}

void DirectionalLight::Update() {
	//lightingResourceに書き込む
	ConstBufferDataDirectionalLight* directionalLightData = nullptr;
	lightingResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));
	directionalLightData->lightingMethod = lightingMethod_;
	directionalLightData->enableLighting = enableLighting_;
	directionalLightData->color = color_;
	directionalLightData->direction = direction_;
	directionalLightData->intensity = intensity_;
	lightingResource_->Unmap(0, nullptr);
}

void DirectionalLight::SetGraphicsCommand(UINT rootParameterIndex) {
	//lightingResourceの場所を設定
	FCS::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(rootParameterIndex, lightingResource_->GetGPUVirtualAddress());
}