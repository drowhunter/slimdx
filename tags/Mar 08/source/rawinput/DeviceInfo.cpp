/*
* Copyright (c) 2007-2008 SlimDX Group
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

#include "DeviceInfo.h"
#include "MouseInfo.h"
#include "HidInfo.h"
#include "KeyboardInfo.h"
#include "../StackAlloc.h"

#include <windows.h>

using namespace System;
using namespace System::Runtime::InteropServices;

namespace SlimDX
{
namespace RawInput
{
	DeviceInfo::DeviceInfo( RAWINPUTDEVICELIST deviceInfo )
	{
		handle = static_cast<IntPtr>( deviceInfo.hDevice );
		type = static_cast<InputType>( deviceInfo.dwType );

		// Get device name
		UINT size;
		if( ::GetRawInputDeviceInfo( deviceInfo.hDevice, RIDI_DEVICENAME, NULL, &size ) != 0 )
			throw gcnew InvalidOperationException( "Unable to get length of device name" );

		stack_vector<WCHAR> str(size);
		
		UINT result = ::GetRawInputDeviceInfo( deviceInfo.hDevice, RIDI_DEVICENAME, &str[0], &size );
		if( result == -1 )
			throw gcnew InvalidOperationException( "Not enough memory" );
		else if ( result != size )
			throw gcnew InvalidOperationException( "Sizes do not match" );
		
		name = Marshal::PtrToStringAuto( static_cast<IntPtr>( &str[0] ) );

		// Get device info
		if( ::GetRawInputDeviceInfo( deviceInfo.hDevice, RIDI_DEVICEINFO, NULL, &size ) != 0 )
			throw gcnew InvalidOperationException( "Unable to get size of general device info" );

		std::auto_ptr<RID_DEVICE_INFO> rawDeviceInfo(new RID_DEVICE_INFO);
		rawDeviceInfo->cbSize = sizeof(RID_DEVICE_INFO);
		if( ::GetRawInputDeviceInfo( deviceInfo.hDevice, RIDI_DEVICEINFO, rawDeviceInfo.get(), &size ) != size )
			throw gcnew InvalidOperationException( "Unable to get general device info" );

		switch( deviceInfo.dwType )
		{
		case RIM_TYPEMOUSE:
			mouseInfo = gcnew MouseInfo( rawDeviceInfo->mouse );
			break;

		case RIM_TYPEKEYBOARD:
			keyboardInfo = gcnew KeyboardInfo( rawDeviceInfo->keyboard );
			break;

		case RIM_TYPEHID:
			hidInfo = gcnew HidInfo( rawDeviceInfo->hid );
			break;
		}
	}

	System::IntPtr DeviceInfo::Handle::get()
	{
		return handle;
	}
	
	InputType DeviceInfo::Type::get()
	{
		return type;
	}

	System::String^ DeviceInfo::Name::get()
	{
		return name;
	}

	MouseInfo^ DeviceInfo::Mouse::get()
	{
		return mouseInfo;
	}

	KeyboardInfo^ DeviceInfo::Keyboard::get()
	{
		return keyboardInfo;
	}

	HidInfo^ DeviceInfo::Hid::get()
	{
		return hidInfo;
	}
}
}