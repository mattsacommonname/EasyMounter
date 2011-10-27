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