#include "Block.h"

Block::Block()
{
}

Block::~Block(){

}

void Block::Initialize(WorldTransform worldTransform){
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	worldTransform_.translation_ = worldTransform.translation_;
	texHandle_ = TextureManager::Load("Resources/uvChecker.png");

	model_.reset(Model::CreateFromOBJ("Resources","cube.obj"));

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

void Block::Update(){
	worldTransform_.UpdateMatrix();
	viewProjection_.UpdateMatrix();

	worldTransform_.translation_.y -= foolSpeed_;

	if (worldTransform_.translation_.y<=-10) {
		foolSpeed_ = 0;
	}
}

void Block::Draw(ViewProjection viewProjection_){

	model_->Draw(worldTransform_, viewProjection_, texHandle_);

	ImGui::Begin("pos");
	ImGui::End();

	
}
