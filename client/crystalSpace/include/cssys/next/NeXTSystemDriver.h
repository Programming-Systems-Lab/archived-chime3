#ifndef __NeXT_NeXTSystemDriver_h
#define __NeXT_NeXTSystemDriver_h
//=============================================================================
//
//	Copyright (C)1999-2001 by Eric Sunshine <sunshine@sunshineco.com>
//
// The contents of this file are copyrighted by Eric Sunshine.  This work is
// distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE.  You may distribute this file provided that this
// copyright notice is retained.  Send comments to <sunshine@sunshineco.com>.
//
//=============================================================================
//-----------------------------------------------------------------------------
// NeXTSystemDriver.h
//
//	NeXT-specific hardware & operating/system drivers for Crystal Space.
//	This file contains code which is shared between MacOS/X, MacOS/X
//	Server 1.0 (Rhapsody), OpenStep, and NextStep.  See NeXTDelegate.h for
//	the platform-specific portion of the system driver.
//
//-----------------------------------------------------------------------------
#if defined(__cplusplus)

#include "cssys/system.h"
#include "ievent.h"
typedef void* NeXTDelegateHandle;

class NeXTSystemDriver : public csSystemDriver
    {
    typedef csSystemDriver superclass;

private:
    NeXTDelegateHandle controller;	// Application & window delegate.
    iEventOutlet* event_outlet;		// Shared event outlet.

    void init_menu( iConfigFileNew* );
    void timer_fired();
    bool continue_looping() const { return (!ExitLoop && continue_running()); }
    bool continue_running() const { return !Shutdown; }

public:
    DECLARE_IBASE_EXT(csSystemDriver);

    NeXTSystemDriver();
    virtual ~NeXTSystemDriver();
    virtual bool Initialize( int argc, char const* const argv[], char const* );
    virtual bool SystemExtension( char const*, ... );
    virtual void Loop();

    // Implement iEventPlug interface.
    struct NeXTSystemEventPlug : public iEventPlug
	{
	DECLARE_EMBEDDED_IBASE(NeXTSystemDriver);
	virtual uint GetPotentiallyConflictingEvents();
	virtual uint QueryEventPriority( uint type );
	} scfiEventPlug;
    };

class SysSystemDriver : public NeXTSystemDriver {};

#else // __cplusplus

#define NSD_PROTO(RET,FUNC) extern RET NeXTSystemDriver_##FUNC

typedef void* NeXTSystemDriver;

NSD_PROTO(int,system_extension)
    (NeXTSystemDriver, char const* msg, void* data1, void* data2, void* data3);

#undef NSD_PROTO

#endif // __cplusplus

#endif // __NeXT_NeXTSystemDriver_h
