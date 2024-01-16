#include "Block.h"

Block::~Block() {

}

void Block::Init() {
	model_.reset(Model::CreateFromOBJ("Resources", "block.obj"));
}

void Block::Update() {

}

void Block::Draw(const ViewProjection& viewProjection) {

}