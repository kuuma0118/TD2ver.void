#include "Block.h"

Block::Block()
{
}

Block::~Block(){

}

void Block::Initialize(){
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	texHandle_ = TextureManager::Load("Resources/uvChecker.png");

	model_.reset(Model::CreateFromOBJ("Resources","cube.obj"));

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

void Block::Update(){
	worldTransform_.UpdateMatrix();
	viewProjection_.UpdateMatrix();
}

void Block::Draw(ViewProjection viewProjection_){

	model_->Draw(worldTransform_, viewProjection_, texHandle_);
}
