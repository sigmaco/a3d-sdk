ReadMe file for A3D SDK 1.2

What's new in 1.2:
------------------

1.  A new IA3d2 interface was created to carry 4 
    new functions.

      IA3d2::RegisterVersion
      IA3d2::GetSoftwareCaps
      IA3d2::GetHardwareCaps

2.  Static ia3d.lib and sources

The DLL stub library a3d.lib is now replaced by 
the static library ia3d.lib.  This static libary 
is equivalent to the DirectSound libary 
dsound.lib.  ia3d.lib has 3 new functions
and an update of an old function.

New functions include: 

  A3dInitialize 
  A3dUninitialize.

A3dCreate now uses the Windows COM call: CoCreateInstance
to access the a3d.dll.  It returns an additional
success code A3D_OK_OLD_DLL.  Developers should
check for this return status and warn the user of this
situation.  The user should down load the latest A3D
drivers to ensure the best results.

Linking to the static library requires the following
additional libraries: dsound.lib ole32.lib.
You will need to create a guids.c file to
declare the DirectSound GUID, IID_IDirectSound.

Source for the library is also available in ia3d.c.

3.  a3d.dll version 1.2

In addition to servicing the new IA3d2 interface, 
the new a3d.dll adds an additional mode to 
resource management called DYNAMIC_LOOPERS
which improves handling of looping wave files in hardware.

Thread safety
-------------

The static library ia3d.lib is tread safe and is compiled as
using the multithread C library.

This version of a3d.dll is not thread safe.  Only one
thread should be responsible for initializing the A3D COM
object.

Installation
-----------

The SDK installation will install the latest 
a3d.dll and a3d.vxd. You should also install 
DirectX along with the A3D SDK installation.

---------------------------------------------------------
Aureal3D and A3D are trademarks of Aureal Incorporated
DirectSound is a trademark of Microsoft Corporation.