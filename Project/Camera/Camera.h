#pragma once
#include "3D/Matrix/ViewProjection.h"
#include "3D/Matrix/WorldTransform.h"
//
//class Camera {
//public:
//	/// 
//	/// Default Method
//	/// 
//
//	// ������
//	void Initialize();
//
//	// �X�V����
//	void Update();
//
//	/// 
//	/// User Method
//	/// 
//
//	/// Getter
//	/// �r���[�v���W�F�N�V�������擾
//	const ViewProjection& GetViewProjection() { return viewProjection_; }
//
//	/// Setter
//	// �Ǐ]����Ώۂ�ݒ�
//	void SetTarget(const WorldTransform* target) { target_ = target; }
//
//private:
//	ViewProjection viewProjection_;
//	const WorldTransform* target_ = nullptr;
//};