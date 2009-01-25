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

#define DEFINE_ENUM_FLAG_OPERATORS(x)

#include <d2d1.h>
#include <d2d1helper.h>

#include "Direct2DException.h"

#include "RenderTarget.h"
#include "TransformedGeometry.h"

const IID IID_ID2D1TransformedGeometry = __uuidof(ID2D1TransformedGeometry);

using namespace System;

namespace SlimDX
{
namespace Direct2D
{
	TransformedGeometry::TransformedGeometry( ID2D1TransformedGeometry* pointer )
	{
		Construct( pointer );
	}
	
	TransformedGeometry::TransformedGeometry( IntPtr pointer )
	{
		Construct( pointer, NativeInterface );
	}
	
	TransformedGeometry^ TransformedGeometry::FromPointer( ID2D1TransformedGeometry* pointer )
	{
		if( pointer == 0 )
			return nullptr;

		TransformedGeometry^ tableEntry = safe_cast<TransformedGeometry^>( ObjectTable::Find( static_cast<IntPtr>( pointer ) ) );
		if( tableEntry != nullptr )
		{
			pointer->Release();
			return tableEntry;
		}

		return gcnew TransformedGeometry( pointer );
	}

	TransformedGeometry^ TransformedGeometry::FromPointer( IntPtr pointer )
	{
		if( pointer == IntPtr::Zero )
			throw gcnew ArgumentNullException( "pointer" );

		TransformedGeometry^ tableEntry = safe_cast<TransformedGeometry^>( ObjectTable::Find( static_cast<IntPtr>( pointer ) ) );
		if( tableEntry != nullptr )
		{
			return tableEntry;
		}

		return gcnew TransformedGeometry( pointer );
	}

	Matrix3x2 TransformedGeometry::Transform::get()
	{
		Matrix3x2 result;

		InternalPointer->GetTransform( reinterpret_cast<D2D1_MATRIX_3X2_F*>( &result ) );
		return result;
	}

	Geometry^ TransformedGeometry::SourceGeometry::get()
	{
		ID2D1Geometry *geometry = NULL;

		InternalPointer->GetSourceGeometry( &geometry );
		return Geometry::FromPointer( geometry );
	}
}
}