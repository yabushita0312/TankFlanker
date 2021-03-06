﻿#pragma once
#include <DxLib.h>
#include <string_view>
class MV1ModelHandle {
private:
	int handle_;
	MV1ModelHandle(int h) noexcept : handle_(h) {}
	static constexpr int invalid_handle = -1;

public:
	constexpr MV1ModelHandle() noexcept : handle_(invalid_handle) {}
	MV1ModelHandle(const MV1ModelHandle&) = delete;
	MV1ModelHandle(MV1ModelHandle&& o) noexcept : handle_(o.handle_) {
		o.handle_ = invalid_handle;
	}
	MV1ModelHandle& operator=(const MV1ModelHandle&) = delete;
	MV1ModelHandle& operator=(MV1ModelHandle&& o) noexcept {
		this->handle_ = o.handle_;
		o.handle_ = invalid_handle;
		return *this;
	}
	~MV1ModelHandle() noexcept {
		if (-1 != this->handle_) {
			MV1DeleteModel(this->handle_);
		}
	}
	void Dispose() noexcept {
		if (-1 != this->handle_) {
			MV1DeleteModel(this->handle_);
			this->handle_ = -1;
		}
	}
	int get() const noexcept { return this->handle_; }

	VECTOR_ref frame(int p1) const noexcept { return MV1GetFramePosition(this->handle_, p1); }
	size_t material_num(void) const noexcept { return MV1GetMaterialNum(this->handle_); }
	size_t mesh_num(void) const noexcept { return MV1GetMeshNum(this->handle_); }
	size_t frame_num(void) const noexcept { return MV1GetFrameNum(this->handle_); }

	MV1ModelHandle Duplicate() const noexcept { return DxLib::MV1DuplicateModel(this->handle_); }
	static MV1ModelHandle Load(std::basic_string_view<TCHAR> FileName) noexcept {
		return { DxLib::MV1LoadModelWithStrLen(FileName.data(), FileName.length()) };
	}
};
