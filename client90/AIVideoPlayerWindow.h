// AIVideoPlayerWindow.h: interface for the AIVideoPlayerWindow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AIVIDEOPLAYERWINDOW_H__CDD46ABE_5F0B_47A3_8A32_0634F3695EEF__INCLUDED_)
#define AFX_AIVIDEOPLAYERWINDOW_H__CDD46ABE_5F0B_47A3_8A32_0634F3695EEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class csListBox;
class csComponent;
class csButton;
struct iEvent;
//AIVideo Player Window
class AIVideoPlayerWindow : public AlwaysVisibleWindow
{


public:
csButton *PlayBut;
csButton *PauseBut;
csButton *NextBut;
csButton *NetBut;
csButton *TimeBut;
  AIVideoPlayerWindow(csComponent *iParent);
  virtual ~AIVideoPlayerWindow ();
  virtual bool HandleEvent (iEvent &Event);

};


#endif // !defined(AFX_AIVIDEOPLAYERWINDOW_H__CDD46ABE_5F0B_47A3_8A32_0634F3695EEF__INCLUDED_)
