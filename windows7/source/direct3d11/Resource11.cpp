#include "stdafx.h"
/*
* Copyright (c) 2007-2009 SlimDX Group
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

#include <d3d11.h>
#include <d3dx11.h>

#include "Direct3D11Exception.h"

#include "Device11.h"
#include "Resource11.h"
#include "Buffer11.h"
#include "Texture1D11.h"
#include "Texture2D11.h"
#include "Texture3D11.h"

using namespace System;
using namespace System::IO;

namespace SlimDX
{
namespace Direct3D11
{
	Resource^ Resource::FromPointer( ID3D11Resource* pointer )
	{
		if( pointer == NULL )
			return nullptr;

		Resource^ tableEntry = safe_cast<Resource^>( SlimDX::ObjectTable::Find( static_cast<System::IntPtr>( pointer ) ) );
		if( tableEntry != nullptr )
		{
			pointer->Release();
			return tableEntry;
		}

		//not in the table, find out what this thing actually is
		D3D11_RESOURCE_DIMENSION type;
		pointer->GetType( &type );

		// chain to the correct ctor (not the fastest way to do this, but good enough for now)
		switch(type)
		{
		case D3D11_RESOURCE_DIMENSION_BUFFER:
			return Buffer::FromPointer( pointer );
		case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
			return Texture1D::FromPointer( pointer );
		case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
			return Texture2D::FromPointer( pointer );
		case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
			return Texture3D::FromPointer( pointer );

		default:
			throw gcnew InvalidCastException( "Unrecognized resource type." );
		}
	}

	Resource^ Resource::FromPointer( System::IntPtr pointer )
	{
		if( pointer == System::IntPtr::Zero )
			throw gcnew System::ArgumentNullException( "pointer" );

		Resource^ tableEntry = safe_cast<Resource^>( SlimDX::ObjectTable::Find( pointer ) );
		if( tableEntry != nullptr )
			return tableEntry;

		void* result = 0;
		IUnknown* unknown = static_cast<IUnknown*>( pointer.ToPointer() );
		HRESULT hr = unknown->QueryInterface(IID_ID3D11Resource, &result);
		if( FAILED( hr ) )
			throw gcnew InvalidCastException( "Failed to QueryInterface on user-supplied pointer." );

		// find out what this thing actually is
		ID3D11Resource* resource = static_cast<ID3D11Resource*>( unknown );
		D3D11_RESOURCE_DIMENSION type;
		resource->GetType( &type );
		resource->Release();

		// chain to the correct ctor (not the fastest way to do this, but good enough for now)
		switch(type)
		{
		case D3D11_RESOURCE_DIMENSION_BUFFER:
			return Buffer::FromPointer( pointer );
		case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
			return Texture1D::FromPointer( pointer );
		case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
			return Texture2D::FromPointer( pointer );
		case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
			return Texture3D::FromPointer( pointer );

		default:
			throw gcnew InvalidCastException( "Unrecognized resource type." );
		}
	}

	DXGI::ResourcePriority Resource::EvictionPriority::get()
	{
		return static_cast<DXGI::ResourcePriority>( InternalPointer->GetEvictionPriority() );
	}
	
	void Resource::EvictionPriority::set( DXGI::ResourcePriority value )
	{
		InternalPointer->SetEvictionPriority( static_cast<UINT>( value ) );
	}
	
	ResourceDimension Resource::Dimension::get()
	{
		D3D11_RESOURCE_DIMENSION type;
		InternalPointer->GetType(&type);
		return static_cast<ResourceDimension>( type );
	}

	int Resource::GetMipSize( int mipSlice, int baseSliceSize )
	{
		float size = static_cast<float>( baseSliceSize );
		
		while( mipSlice > 0 )
		{
			size = std::floorf(size / 2.0f);
			--mipSlice;
		}
		
		return (static_cast< int >(size));
	}
	
	ID3D11Resource* Resource::ConstructFromFile( SlimDX::Direct3D11::Device^ device, String^ fileName, D3DX11_IMAGE_LOAD_INFO* info )
	{	
		ID3D11Resource* resource = 0;
		pin_ptr<const wchar_t> pinnedName = PtrToStringChars( fileName );
		HRESULT hr = D3DX11CreateTextureFromFile( device->InternalPointer, pinnedName, info, 0, &resource, 0 );
		RECORD_D3D11( hr );
		
		return resource;
	}
	
	ID3D11Resource* Resource::ConstructFromMemory( SlimDX::Direct3D11::Device^ device, array<Byte>^ memory, D3DX11_IMAGE_LOAD_INFO* info )
	{
		pin_ptr<unsigned char> pinnedMemory = &memory[0];
		
		ID3D11Resource* resource = 0;
		HRESULT hr = D3DX11CreateTextureFromMemory( device->InternalPointer, pinnedMemory, memory->Length, info, 0, &resource, 0 ); 
		RECORD_D3D11( hr );
		
		return resource;
	}
	
	ID3D11Resource* Resource::ConstructFromStream( SlimDX::Direct3D11::Device^ device, Stream^ stream, int sizeInBytes, D3DX11_IMAGE_LOAD_INFO* info )
	{
		DataStream^ ds = nullptr;
		array<Byte>^ memory = SlimDX::Utilities::ReadStream( stream, sizeInBytes, &ds );
		
		if( memory == nullptr )
		{
			ID3D11Resource* resource = NULL;
			SIZE_T size = static_cast<SIZE_T>( ds->RemainingLength );
			HRESULT hr = D3DX11CreateTextureFromMemory( device->InternalPointer, ds->SeekToEnd(), size,
				info, NULL, &resource, NULL );
			RECORD_D3D11( hr );

			return resource;
		}

		return ConstructFromMemory( device, memory, info );
	}
}
}
