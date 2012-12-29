/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2011 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#ifndef __RenderTexture_H__
#define __RenderTexture_H__

#include "CmPrerequisites.h"
#include "CmTexture.h"
#include "CmRenderTarget.h"

namespace CamelotEngine
{    
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup RenderSystem
	*  @{
	*/
	/** This class represents a RenderTarget that renders to a Texture. There is no 1 on 1
        relation between Textures and RenderTextures, as there can be multiple 
        RenderTargets rendering to different mipmaps, faces (for cubemaps) or slices (for 3D textures)
        of the same Texture.
    */
    class CM_EXPORT RenderTexture: public RenderTarget
    {
	public:
		virtual ~RenderTexture() {}

		void setBuffers(TexturePtr texture, DepthStencilBufferPtr depthStencilbuffer, 
			UINT32 face = 0, UINT32 numFaces = 0, UINT32 mipLevel = 0);

		TexturePtr getTexture() const { return mTexture; }
		DepthStencilBufferPtr getDepthStencilBuffer() const { return mDepthStencilBuffer; }

		bool requiresTextureFlipping() const { return false; }

	protected:
		TextureType mType;
		PixelFormat mFormat;
		DepthStencilFormat mDepthStencilFormat;
		UINT32 mFace;
		UINT32 mNumFaces;
		UINT32 mMipLevel;

		TexturePtr mTexture;
		DepthStencilBufferPtr mDepthStencilBuffer;

		RenderTexture();

		void initialize(TextureType textureType, UINT32 width, UINT32 height, 
			PixelFormat format, bool hwGamma, UINT32 fsaa, const String& fsaaHint, 
			bool createDepth = true, DepthStencilFormat depthStencilFormat = DFMT_D24S8);

		void initialize(TexturePtr texture, DepthStencilBufferPtr depthStencilbuffer, 
			UINT32 face = 0, UINT32 numFaces = 0, UINT32 mipLevel = 0);

		void createTextureBuffer();
		void createDepthStencilBuffer();

		void createInternalResources();
		virtual void createInternalResourcesImpl() = 0;
    };

	/** This class represents a render target that renders to multiple RenderTextures
		at once. Surfaces can be bound and unbound at will, as long as the following constraints
		are met:
		- All bound surfaces have the same size
		- All bound surfaces have the same bit depth
		- Target 0 is bound
	*/
	class CM_EXPORT MultiRenderTarget: public RenderTarget
	{
	public:
		MultiRenderTarget(const String &name);

		/** Bind a surface to a certain attachment point.
            @param attachment	0 .. mCapabilities->getNumMultiRenderTargets()-1
			@param target		RenderTexture to bind.

			It does not bind the surface and fails with an exception (ERR_INVALIDPARAMS) if:
			- Not all bound surfaces have the same size
			- Not all bound surfaces have the same internal format 
		*/

		virtual void bindSurface(UINT32 attachment, RenderTexture *target)
		{
			for (UINT32 i = (UINT32)mBoundSurfaces.size(); i <= attachment; ++i)
			{
				mBoundSurfaces.push_back(0);
			}
			mBoundSurfaces[attachment] = target;

			bindSurfaceImpl(attachment, target);
		}

		/** Unbind attachment.
		*/
		virtual void unbindSurface(UINT32 attachment)
		{
			if (attachment < (UINT32)mBoundSurfaces.size())
				mBoundSurfaces[attachment] = 0;
			unbindSurfaceImpl(attachment);
		}

		/** Error throwing implementation, it's not possible to write a MultiRenderTarget
			to disk. 
		*/
		virtual void copyContentsToMemory(const PixelData &dst, FrameBuffer buffer);

		/// Irrelevant implementation since cannot copy
		PixelFormat suggestPixelFormat() const { return PF_UNKNOWN; }

		typedef vector<RenderTexture*>::type BoundSufaceList;
		/// Get a list of the surfaces which have been bound
		const BoundSufaceList& getBoundSurfaceList() const { return mBoundSurfaces; }

		/** Get a pointer to a bound surface */
		RenderTexture* getBoundSurface(UINT32 index)
		{
			assert (index < mBoundSurfaces.size());
			return mBoundSurfaces[index];
		}

	protected:
		BoundSufaceList mBoundSurfaces;

		/// implementation of bindSurface, must be provided
		virtual void bindSurfaceImpl(UINT32 attachment, RenderTexture *target) = 0;
		/// implementation of unbindSurface, must be provided
		virtual void unbindSurfaceImpl(UINT32 attachment) = 0;
	};
	/** @} */
	/** @} */
}

#endif
