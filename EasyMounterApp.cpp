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

#include "constants.h"
#include "EasyMounterApp.h"

EasyMounterApp::EasyMounterApp (void)
: BApplication ("application/x-vnd.EasyMounter") {
	settings = new SettingsFile;
	settings->Load ();

	BRect frame;
	if (settings->FindRect ("select_win_frame", & frame) < B_OK) {
		int32 w = DEFAULT_SELECT_WIN_X + SELECTION_WINDOW_MIN_W;
		int32 h = DEFAULT_SELECT_WIN_Y + SELECTION_WINDOW_MIN_H;
		frame = BRect (DEFAULT_SELECT_WIN_X, DEFAULT_SELECT_WIN_Y, w, h);
	}

	select_win = new SelectionWindow (frame, settings);
	select_win->Show ();
}

EasyMounterApp::~EasyMounterApp (void) {
//	settings->Save ();
}

void EasyMounterApp::MessageReceived (BMessage * msg) {
	switch (msg->what) {
		default : {
			BApplication::MessageReceived (msg);
			break;
		}
	}
}