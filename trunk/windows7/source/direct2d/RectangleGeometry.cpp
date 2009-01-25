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
#include "RectangleGeometry.h"

const IID IID_ID2D1RectangleGeometry = __uuidof(ID2D1RectangleGeometry);

using namespace System;

namespace SlimDX
{
namespace Direct2D
{
	RectangleGeometry::RectangleGeometry( ID2D1RectangleGeometry* pointer )
	{
		Construct( pointer );
	}
	
	RectangleGeometry::RectangleGeometry( IntPtr pointer )
	{
		Construct( pointer, NativeInterface );
	}
	
	RectangleGeometry^ RectangleGeometry::FromPointer( ID2D1RectangleGeometry* pointer )
	{
		if( pointer == 0 )
			return nullptr;

		RectangleGeometry^ tableEntry = safe_cast<RectangleGeometry^>( ObjectTable::Find( static_cast<IntPtr>( pointer ) ) );
		if( tableEntry != nullptr )
		{
			pointer->Release();
			return tableEntry;
		}

		return gcnew RectangleGeometry( pointer );
	}

	RectangleGeometry^ RectangleGeometry::FromPointer( IntPtr pointer )
	{
		if( pointer == IntPtr::Zero )
			throw gcnew ArgumentNullException( "pointer" );

		RectangleGeometry^ tableEntry = safe_cast<RectangleGeometry^>( ObjectTable::Find( static_cast<IntPtr>( pointer ) ) );
		if( tableEntry != nullptr )
		{
			return tableEntry;
		}

		return gcnew RectangleGeometry( pointer );
	}

	System::Drawing::RectangleF RectangleGeometry::Rectangle::get()
	{
		D2D1_RECT_F result;

		InternalPointer->GetRect( &result );
		return System::Drawing::RectangleF::FromLTRB( result.left, result.top, result.right, result.bottom );
	}
}
}