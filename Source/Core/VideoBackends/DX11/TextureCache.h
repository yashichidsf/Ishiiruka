// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#pragma once
#include "VideoBackends/DX11/D3DTexture.h"
#include "VideoBackends/DX11/TextureEncoder.h"
#include "VideoCommon/TextureCacheBase.h"

namespace DX11
{

class TextureCache : public ::TextureCache
{
public:
	TextureCache();
	~TextureCache();

private:
	struct TCacheEntry : TCacheEntryBase
	{
		D3DTexture2D *const texture;
		DXGI_FORMAT DXGI_format;
		D3D11_USAGE usage;
		bool swap_rg;
		bool convertrgb565;
		bool compressed;

		TCacheEntry(const TCacheEntryConfig& config, D3DTexture2D *_tex) : TCacheEntryBase(config),
			texture(_tex), swap_rg(false), convertrgb565(false), compressed(false)
		{}
		~TCacheEntry();

		void Load(const u8* src, u32 width, u32 height,
			u32 expanded_width, u32 level);
		void Load(const u8* src, u32 width, u32 height, u32 expandedWidth,
			u32 expandedHeight, const s32 texformat, const u32 tlutaddr, const TlutFormat tlutfmt, u32 level);
		void LoadFromTmem(const u8* ar_src, const u8* gb_src, u32 width, u32 height,
			u32 expanded_width, u32 expanded_Height, u32 level);

		void FromRenderTarget(
			PEControl::PixelFormat srcFormat, const EFBRectangle& srcRect,
			bool isIntensity, bool scaleByHalf, u32 cbufid,
			const float *colmat) override;

		bool PalettizeFromBase(const TCacheEntryBase* base_entry);

		void Bind(u32 stage);
		bool Save(const std::string& filename, u32 level);
	};
	
	PC_TexFormat GetNativeTextureFormat(const s32 texformat, const TlutFormat tlutfmt, u32 width, u32 height);

	TextureCache::TCacheEntryBase* CreateTexture(const TCacheEntryConfig& config);

	u64 EncodeToRamFromTexture(u32 address, void* source_texture, u32 SourceW, u32 SourceH, bool bFromZBuffer, bool bIsIntensityFmt, u32 copyfmt, int bScaleByHalf, const EFBRectangle& source) {return 0;};
	void LoadLut(u32 lutFmt, void* addr, u32 size) override;
	void CompileShaders() override { }
	void DeleteShaders() override { }
};

}
