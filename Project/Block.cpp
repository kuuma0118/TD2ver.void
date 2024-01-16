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

	AdjustmentParameter();

	worldTransform_.translation_.y -= foolSpeed_;

	if (worldTransform_.translation_.y<=-10) {
		foolSpeed_ = 0;
	}
}

void Block::Draw(ViewProjection viewProjection_){

	model_->Draw(worldTransform_, viewProjection_, texHandle_);
}

void Block::AdjustmentParameter()
{
#ifdef _DEBUG
	ImGui::Begin("Player");

	/*if (ImGui::TreeNode("worldTransform")) {
		ImGui::DragFloat3("translate", &worldTransform_.translation_.x, 0.1f, 100, 100);
		ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat3("scale", &worldTransform_.scale_.x, 0.01f, 0, 10);
		ImGui::TreePop();
	}*/
	/*if (ImGui::TreeNode("Inverse Velocity")) {
		ImGui::DragFloat3("velocity", &velocity_.x, 0.01f, -1.0f, 1.0f);
		if (ImGui::Button("isActive")) {
			inverseVelSignal_ = true;
		}
		ImGui::TreePop();
	}*/


	ImGui::End();
#endif // DEBUG
}
