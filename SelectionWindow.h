/*
 * Copyright © 2011 Matthew Bishop
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef SELECTION_WINDOW_H
#define SELECTION_WINDOW_H

#include <Button.h>
#include <ListView.h>
#include <Window.h>

#include "MountItem.h"
#include "SettingsFile.h"

class SelectionWindow
: public BWindow {
	public:
		SelectionWindow (BRect, SettingsFile *);
		virtual void FrameMoved (BPoint);
		virtual void FrameResized (float, float);
		virtual void MessageReceived (BMessage *);
		void SetButtonsEnabled (bool);
		void AddMount (MountItem *);
		void EditMount (int32, MountItem *);
		void DeleteMount (int32);
		bool Mount (int32);
		void Unmount (int32);
		void LoadMounts (void);
		void Save (void);
		void HelpRequested (void);
		virtual bool QuitRequested (void);
		void UpdateSettingsMounts (int32 = -1, bool = false);
		void UpdateSettingsFrame (void);
	private:
		BListView * mounts_list;
		SettingsFile * settings;
		BButton * mount_button, * edit_button, * delete_button;
};

#endif