/*
 * Copyright 2015 Matthew Bishop
 *  
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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