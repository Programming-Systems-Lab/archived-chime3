/*
    Copyright (C) 1998-2000 by Jorrit Tyberghein

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef __CS_SYSTEM_H__
#define __CS_SYSTEM_H__

#include <stdio.h>

#include "csutil/scf.h"
#include "csutil/util.h"
#include "csutil/csstrvec.h"
#include "csutil/csobjvec.h"
#include "cssys/csinput.h"
#include "cssys/csevcord.h"
#include "isystem.h"
#include "ivfs.h"
#include "ievent.h"
#include "iconfig.h"

class csKeyboardDriver;
class csMouseDriver;

struct iGraphics3D;
struct iGraphics2D;
struct iNetworkDriver;
struct iSoundRender;
struct iConfig;
struct iConsole;
struct iConfigFile;
struct iMotionManager;

/**
 * This is the interface to operating system.<p>
 * This driver takes care of all system-dependent parts such as
 * video hardware and input hardware. Note that system-dependent
 * source code should NOT write implementations for methods of
 * csSystemDriver (they are already implemented in system.cpp),
 * but inherit a new class from csSystemDriver, overriding desired
 * methods. Note that some methods it is required to override,
 * otherwise program simply will not compile (they are marked
 * as abstract).
 * <p>
 * This is an abstract class since it does not implement the iBase
 * interface. The iBase interface is supposed to be implemented
 * in SysSystemDriver which should be derived from csSystemDriver.
 */
class csSystemDriver : public iSystem
{
  /*
   * This is a private structure used to keep the list of plugins.
   */
  class csPlugIn : public csBase
  {
  public:
    // The plugin itself
    iPlugIn *PlugIn;
    // The class ID of the plugin, and their functionality ID
    char *ClassID, *FuncID;
    // The mask of events this plugin wants to see
    unsigned int EventMask;

    // Construct the object that represents a plugin
    csPlugIn (iPlugIn *iObject, const char *iClassID, const char *iFuncID);
    // Free storage
    virtual ~csPlugIn ();
  };

  /*
   * This is a superset of csObjVector that can find by pointer a plugin.
   */
  class csPlugInsVector : public csObjVector
  {
  public:
    // Create the vector
    csPlugInsVector (int iLimit, int iDelta) : csObjVector (iLimit, iDelta) {}
    // Find a plugin either by its address or by his function ID
    virtual int CompareKey (csSome Item, csConstSome Key, int Mode) const
    {
      if (Mode == 0)
        return ((csPlugIn *)Item)->PlugIn == Key ? 0 : 1;
      else
        return ((csPlugIn *)Item)->FuncID ? strcmp (((csPlugIn *)Item)->FuncID, (char *)Key)
             : ((csPlugIn *)Item)->FuncID == Key ? 0 : 1;
    }
    // Overrided Get() to avoid typecasts
    csPlugIn *Get (int idx)
    { return (csPlugIn *)csObjVector::Get (idx); }
  };

  /*
   * Class to collect all options for all plug-in modules in the system.
   */
  class csPluginOption : public csBase
  {
  public:
    char *Name;
    csVariantType Type;
    int ID;
    bool Value;				// If Type is CSVAR_BOOL
    iConfig *Config;

    csPluginOption (const char *iName, csVariantType iType, int iID, bool iValue, iConfig* iConfig)
    {
      Name = strnew (iName);
      Type = iType;
      ID = iID;
      Value = iValue;
      (Config = iConfig)->IncRef ();
    }
    virtual ~csPluginOption ()
    {
      Config->DecRef ();
      delete [] Name;
    }
  };

  /*
   * This structure contains the data related to one command-line option.
   */
  struct csCommandLineOption
  {
    /// Option name
    char *Name;
    /// Option value
    char *Value;
    /// Name and Value should be already allocated
    csCommandLineOption (char *iName, char *iValue)
    { Name = iName; Value = iValue; }
    /// Destructor
    ~csCommandLineOption ()
    { delete [] Name; delete [] Value; }
  };

  /*
   * The array of all command-line options.
   */
  class csCommandLineOptions : public csVector
  {
  public:
    csCommandLineOptions (int iLength, int iDelta) : csVector (iLength, iDelta) {}
    virtual ~csCommandLineOptions ()
    { DeleteAll (); }
    virtual bool FreeItem (csSome Item)
    { delete (csCommandLineOption *)Item; return true; }
    virtual int CompareKey (csSome Item, csConstSome Key, int /*Mode*/) const
    { return strcmp (((csCommandLineOption *)Item)->Name, (const char*)Key); }
  };

  // Find Nth command-line option and return a pointer to the object (or NULL)
  csCommandLineOption *FindOptionCL (const char *iName, int iIndex);

  /*
   * A private class which implements the iEventOutlet interface.
   */
  class csEventOutlet : public iEventOutlet
  {
    // The mask of events to allow from this plug
    unsigned EnableMask;
    // The event plug object
    iEventPlug *Plug;
    // The system driver
    csSystemDriver *System;
  public:
    DECLARE_IBASE;

    // Initialize the outlet
    csEventOutlet (iEventPlug *iPlug, csSystemDriver *iSys);
    // Destroy the outlet
    virtual ~csEventOutlet ();

    // Create a event object on behalf of the system driver.
    virtual iEvent *CreateEvent ();
    // Put a previously created event into system event queue.
    virtual void PutEvent (iEvent *Event);
    // Put a keyboard event into event queue.
    virtual void Key (int iKey, int iChar, bool iDown);
    // Put a mouse event into event queue.
    virtual void Mouse (int iButton, bool iDown, int x, int y);
    // Put a joystick event into event queue.
    virtual void Joystick (int iNumber, int iButton, bool iDown, int x, int y);
    // Put a broadcast event into event queue.
    virtual void Broadcast (int iCode, void *iInfo);
    // Broadcast a event to all plugins
    virtual void ImmediateBroadcast (int iCode, void *iInfo);
  };
  friend class csEventOutlet;

  /*
   * The array of all allocated event outlets.
   */
  class csEventOutletsVector : public csVector
  {
  public:
    csEventOutletsVector () : csVector (16, 16)
    { }
    virtual ~csEventOutletsVector ()
    { DeleteAll (); }
    virtual bool FreeItem (csSome Item)
    { delete (csEventOutlet *)Item; return true; }
    csEventOutlet *Get (int idx)
    { return (csEventOutlet *)csVector::Get (idx); }
  } EventOutlets;

  /*
   * The array of all allocated event cords.
   */
  class csEventCordsVector : public csVector
  {
  public:
    csEventCordsVector () : csVector (16, 16)
    { }
    virtual ~csEventCordsVector ()
    { DeleteAll (); }
    virtual bool FreeItem (csSome Item)
    { delete (csEventCord *)Item; return true; }
    csEventCord *Get (int idx)
    { return (csEventCord *)csVector::Get (idx); }
    int Find (int Category, int SubCategory);
  } EventCords;

  // Collect all options from command line
  virtual void CollectOptions (int argc, const char* const argv[]);

  // Query all options supported by given plugin and place into OptionList
  void QueryOptions (iPlugIn *iObject);

  // Elapsed time between last two frames and absolute time in milliseconds
  cs_time ElapsedTime, CurrentTime;

public:
  /// -------------------------- plug-ins --------------------------
  /// The list of all plug-ins
  csPlugInsVector PlugIns;
  /// The main configuration file
  iConfigFileNew *Config;
  /// The Virtual File System object
  iVFS *VFS;
  /// 3D Graphics context
  iGraphics3D* G3D;
  /// 2D Graphics context
  iGraphics2D* G2D;
  /// Sound render
  iSoundRender* Sound;
  /// Network driver
  iNetworkDriver* NetDrv;
  /// System console
  iConsole *Console;
  /// Motion manager
  iMotionManager *MotionMan;

  /// the width of this frame
  int FrameWidth;
  /// the height of this frame
  int FrameHeight;
  /// whether this device is full-screen or not.
  bool FullScreen;
  /// the bits-per-pixel of the display.
  int Depth;
  /// The event queue
  csEventQueue EventQueue;
  /// Keyboard driver
  csKeyboardDriver Keyboard;
  /// Mouse driver
  csMouseDriver Mouse;
  /// Joystick driver
  csJoystickDriver Joystick;
  /// Set to non-zero to exit csSystemDriver::Loop()
  bool Shutdown;
  /// Same as Shutdown but set manually by windowing system
  bool ExitLoop;
  /// Debugging level (0 = no debug, 1 = normal debug, 2 = verbose debug)
  int debug_level;
  /// List of all options for all plug-in modules.
  csObjVector OptionList;
  /// The collection of all options specified on command line
  csCommandLineOptions CommandLine;
  /// The list of raw filenames on the command line (i.e. without any switches)
  csStrVector CommandLineNames;

  /// Initialize system-dependent data
  csSystemDriver ();
  /// Deinitialize system-dependent parts
  virtual ~csSystemDriver ();

  /// This is usually called right after object creation.
  virtual bool Initialize (int argc, const char* const argv[], const char *iConfigName);

  /// Check if all required drivers are loaded
  virtual bool CheckDrivers ();

  /**
   * Open the graphics context (with optional title on titlebar),
   * mouse and keyboard.
   */
  virtual bool Open (const char *Title);
  /// Close the system
  virtual void Close ();

  /**
   * System loop. This should be called last since it returns
   * only on program exit. There are two ways for every Crystal Space
   * application to function: the simplest way is to call Loop() and
   * it will take care of everything. The second way is to call NextFrame
   * manually often enough, and use an API-dependent application loop
   * (many GUI toolkits require your application to call their own event
   * loop function rather than providing your own). You will have to handle
   * the Shutdown variable yourself then.
   */
  virtual void Loop ();

  /**
   * SysSystemDriver::Loop calls this method once per frame.
   * This method can be called manually as well if you don't use the
   * Loop method. System drivers may override this method to pump events
   * from system event queue into Crystal Space event queue.
   */
  virtual void NextFrame ();

  /// Pass a single event to all plugins until one eats it
  virtual bool HandleEvent (iEvent &Event);

  /// Sleep for given number of 1/1000 seconds (very inacurate)
  virtual void Sleep (int /*SleepTime*/) {}

  /**
   * This is a function that prints the commandline help text.
   * If system driver has system-dependent switches, it should override
   * this method and type its own text (possibly after invoking
   * csSystemDriver::Help() first).
   */
  virtual void Help ();

  /**
   * Return time in milliseconds (if not supported by a system
   * just return the time in seconds multiplied by 1000).
   */
  static cs_time Time ();

  /// Print a string into debug.txt
  static void debug_out (bool flush, const char* str);

  /**
   * Printf version that works on all systems.
   * Default implementation is in 'libs/cssys/general/'.
   */
  static void console_out (const char *str);

  /// A shortcut for requesting to load a plugin (before Initialize())
  void RequestPlugin (const char *iPluginName);

protected:
  /**
   * Print help for an iConfig interface.
   */
  void Help (iConfig* Config);

  /**
   * Show an alert. This function is called by CsPrintf and
   * should show an alert in case of an error.
   * The default implementation writes the message in debug.txt
   * and on standard output.
   */
  virtual void Alert (const char* msg);

  /**
   * Show a warning. This function is called by CsPrintf and
   * should show an alert in case of a warning.
   * The default implementation writes a message in debug.txt
   * and on standard output.
   */
  virtual void Warn (const char* msg);

  /**
   * Query default width/height/depth from config file
   * and from command-line parameters.
   */
  virtual void SetSystemDefaults (iConfigFileNew *config);

public:
  DECLARE_IBASE;

  /**************************** iSystem interface ****************************/

  /// Returns the configuration.
  virtual void GetSettings (int &oWidth, int &oHeight, int &oDepth, bool &oFullScreen);
  /// Get the time in milliseconds.
  virtual cs_time GetTime ();
  /// Print a string to the specified device.
  virtual void Printf (int mode, const char *format, ...);
  /// Execute a system-dependent extension command.
  virtual bool SystemExtension (const char *iCommand, ...)
  { (void)iCommand; return false; }
  /// Suspend the engine's virtual-time clock.
  virtual void SuspendVirtualTimeClock() {}
  /// Resume the engine's virtual-time clock.
  virtual void ResumeVirtualTimeClock() { CurrentTime = cs_time(-1); }
  /// Query the elapsed time between last frames and absolute time.
  virtual void GetElapsedTime (cs_time &oElapsedTime, cs_time &oCurrentTime)
  { oElapsedTime = ElapsedTime; oCurrentTime = CurrentTime; }
  /// Get the installation path.
  virtual bool GetInstallPath (char *oInstallPath, size_t iBufferSize);

  /// Load a plugin and initialize it
  virtual iBase *LoadPlugIn (const char *iClassID, const char *iFuncID,
    const char *iInterface, int iVersion);
  /// Get first of the loaded plugins that supports given interface ID
  virtual iBase *QueryPlugIn (const char *iInterface, int iVersion);
  /// Find a plugin given his functionality ID
  virtual iBase *QueryPlugIn (const char *iFuncID, const char *iInterface, int iVersion);
  /// Find a plugin given his class ID and functionality ID
  virtual iBase *QueryPlugIn (const char* iClassID, const char *iFuncID, const char *iInterface, int iVersion);
  /// Remove a plugin from system driver's plugin list
  virtual bool UnloadPlugIn (iPlugIn *iObject);
  /// Register a object that implements the iPlugIn interface as a plugin
  virtual bool RegisterPlugIn (const char *iClassID, const char *iFuncID, iPlugIn *iObject);
  /// Get the number of loaded plugins in the plugin manager.
  virtual int GetNumPlugIns ();
  /// Get the specified plugin from the plugin manager.
  virtual iBase* GetPlugIn (int idx);

  /// Get the system configuration file: this does NOT IncRef the object
  virtual iConfigFileNew *GetConfig ();
  /// Create a new configuration file object which resides on VFS
  virtual iConfigFile *CreateConfig (const char *iFileName, bool iVFS = true);
  /**
   * Create a new configuration file object which resides on VFS. This method
   * uses a new format for the config files, not the INI format.
   */
  virtual iConfigFileNew *CreateConfigNew (const char *iFileName, bool iVFS = true);
  /// Save system configuration file if it was changed
  virtual bool SaveConfig ();

  /// Register the plugin to receive specific events
  virtual bool CallOnEvents (iPlugIn *iObject, unsigned int iEventMask);
  /// Query current state for given key
  virtual bool GetKeyState (int key);
  /// Query current state for given mouse button (0..9)
  virtual bool GetMouseButton (int button);
  /// Query current (last known) mouse position
  virtual void GetMousePosition (int &x, int &y);
  /// Query current state for given joystick button (1..CS_MAX_JOYSTICK_BUTTONS)
  virtual bool GetJoystickButton (int number, int button);
  /// Query last known joystick position
  virtual void GetJoystickPosition (int number, int &x, int &y);
  /// Register an event plug and return a new outlet
  virtual iEventOutlet *CreateEventOutlet (iEventPlug *iObject);
  /// Get an event cord for the given category/subcategory.
  virtual iEventCord *GetEventCord(int Category, int Subcategory);
  /// Get a public event outlet for posting just a single event and such.
  virtual iEventOutlet *GetSystemEventOutlet ();

  /// Query a specific commandline option (you can query second etc such option)
  virtual const char *GetOptionCL (const char *iName, int iIndex = 0);
  /// Query a filename specified on the commandline (that is, without leading '-')
  virtual const char *GetNameCL (int iIndex = 0);
  /// Add a command-line option to the command-line option array
  virtual void AddOptionCL (const char *iName, const char *iValue);
  /// Add a command-line name to the command-line names array
  virtual void AddNameCL (const char *iName);
  /// Replace the Nth command-line option with a new value
  virtual bool ReplaceOptionCL (const char *iName, const char *iValue, int iIndex = 0);
  /// Replace the Nth command-line name with a new value
  virtual bool ReplaceNameCL (const char *iValue, int iIndex = 0);

  /****************************** iSCF interface ******************************/

  class csSCF : public iSCF
  {
  public:
    DECLARE_EMBEDDED_IBASE (csSystemDriver);

    /// Wrapper for scfClassRegistered ()
    virtual bool scfClassRegistered (const char *iClassID);
    /// Wrapper for scfCreateInstance ()
    virtual void *scfCreateInstance (const char *iClassID, const char *iInterfaceID,
      int iVersion);
    /// Wrapper for scfGetClassDescription ()
    virtual const char *scfGetClassDescription (const char *iClassID);
    /// Wrapper for scfGetClassDependencies ()
    virtual const char *scfGetClassDependencies (const char *iClassID);
    /// Wrapper for scfRegisterClass ()
    virtual bool scfRegisterClass (const char *iClassID, const char *iLibraryName,
      const char *Dependencies = NULL);
    /// Wrapper for scfRegisterStaticClass ()
    virtual bool scfRegisterStaticClass (scfClassInfo *iClassInfo);
    /// Wrapper for scfRegisterClassList ()
    virtual bool scfRegisterClassList (scfClassInfo *iClassInfo);
    /// Wrapper for scfUnregisterClass ()
    virtual bool scfUnregisterClass (char *iClassID);
  } scfiSCF;
};

// CrystalSpace global variables
extern csSystemDriver *System;

// Fatal exit routine (which can be replaced if neccessary)
extern void (*fatal_exit) (int errorcode, bool canreturn);

#endif // __CS_SYSTEM_H__
