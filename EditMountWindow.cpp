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

#include <Alert.h>
#include <Application.h>
#include <Button.h>
#include <NetAddress.h>
#include <String.h>

#include "constants.h"
#include "EditMountWindow.h"
#include "SelectionWindow.h"

EditMountWindow::EditMountWindow (BWindow * win, BRect frame)
: BWindow (BRect (frame.left + 10, frame.top + 10, frame.left + EDIT_MOUNT_WINDOW_W + 10, frame.top + 169 + 10), "Add Mount", B_FLOATING_WINDOW, B_NOT_CLOSABLE | B_NOT_ZOOMABLE | B_NOT_RESIZABLE | B_ASYNCHRONOUS_CONTROLS) {
	select_win = win;
	index = -1;

	BRect temp = Bounds ();
	temp.top += 5;
	temp.left += 5;
	temp.bottom = temp.top + 15;
	temp.right -= 5;
	AddChild (ip = new BTextControl (temp, "ip_text", "IP", "127.0.0.1", NULL));
	ip->SetDivider (60);
	temp.top += 19;
	temp.bottom += 19;
	AddChild (workgroup = new BTextControl (temp, "workgroup_text", "Workgroup", "WORKGROUP", NULL));
	workgroup->SetDivider (60);
	temp.top += 19;
	temp.bottom += 19;
	AddChild (host = new BTextControl (temp, "host_text", "Host", "HOST", NULL));
	host->SetDivider (60);
	temp.top += 19;
	temp.bottom += 19;
	AddChild (share = new BTextControl (temp, "share_text", "Share", "SHARE", NULL));
	share->SetDivider (60);
	temp.top += 19;
	temp.bottom += 19;
	AddChild (user = new BTextControl (temp, "user_text", "Username", "user", NULL));
	user->SetDivider (60);
	temp.top += 19;
	temp.bottom += 19;
	AddChild (pass = new BTextControl (temp, "pass_text", "Password", "pass", NULL));
	pass->SetDivider (60);
	temp.top += 19;
	temp.bottom += 19;
	AddChild (mount = new BTextControl (temp, "mount_text", "Local Mount", "/mount", NULL));
	mount->SetDivider (60);

	temp = Bounds ();
	temp.bottom -= 5;
	temp.right -= 5;
	temp.top = temp.bottom - 25;
	temp.left = temp.right - 60;
	AddChild (new BButton (temp, "add_button", "Add", new BMessage (ADD_MOUNT)));
	temp.left -= 65;
	temp.right -= 65;
	AddChild (new BButton (temp, "cancel_button", "Cancel", new BMessage (B_QUIT_REQUESTED)));
}

EditMountWindow::EditMountWindow (BWindow * win, BRect frame, int32 x, const char * i, const char * w, const char * h, const char * s, const char * u, const char * p, const char * m)
: BWindow (BRect (frame.left + 10, frame.top + 10, frame.left + EDIT_MOUNT_WINDOW_W + 10, frame.top + 169 + 10), "Edit Mount", B_FLOATING_WINDOW, B_NOT_CLOSABLE | B_NOT_ZOOMABLE | B_NOT_RESIZABLE | B_ASYNCHRONOUS_CONTROLS) {
	select_win = win;
	index = x;

	BRect temp = Bounds ();
	temp.top += 5;
	temp.left += 5;
	temp.bottom = temp.top + 15;
	temp.right -= 5;
	AddChild (ip = new BTextControl (temp, "ip_text", "IP", i, NULL));
	ip->SetDivider (60);
	temp.top += 19;
	temp.bottom += 19;
	AddChild (workgroup = new BTextControl (temp, "workgroup_text", "Workgroup", w, NULL));
	workgroup->SetDivider (60);
	temp.top += 19;
	temp.bottom += 19;
	AddChild (host = new BTextControl (temp, "host_text", "Host", h, NULL));
	host->SetDivider (60);
	temp.top += 19;
	temp.bottom += 19;
	AddChild (share = new BTextControl (temp, "share_text", "Share", s, NULL));
	share->SetDivider (60);
	temp.top += 19;
	temp.bottom += 19;
	AddChild (user = new BTextControl (temp, "user_text", "Username", u, NULL));
	user->SetDivider (60);
	temp.top += 19;
	temp.bottom += 19;
	AddChild (pass = new BTextControl (temp, "pass_text", "Password", p, NULL));
	pass->SetDivider (60);
	temp.top += 19;
	temp.bottom += 19;
	AddChild (mount = new BTextControl (temp, "mount_text", "Local Mount", m, NULL));
	mount->SetDivider (60);

	temp = Bounds ();
	temp.bottom -= 5;
	temp.right -= 5;
	temp.top = temp.bottom - 25;
	temp.left = temp.right - 60;
	AddChild (new BButton (temp, "add_button", "Edit", new BMessage (ADD_MOUNT)));
	temp.left -= 65;
	temp.right -= 65;
	AddChild (new BButton (temp, "cancel_button", "Cancel", new BMessage (B_QUIT_REQUESTED)));
}

void EditMountWindow::MessageReceived (BMessage * msg) {
	switch (msg->what) {
		case ADD_MOUNT : {
			Lock ();
			
			if (! BString (workgroup->Text ()).CountChars () || ! BString (host->Text ()).CountChars () || ! BString (share->Text ()).CountChars () || ! BString (user->Text ()).CountChars () || ! BString (pass->Text ()).CountChars () || ! BString (mount->Text ()).CountChars ()) {
				(new BAlert ("!", "Please fill in all fields.", "Ok"))->Go ();
				break;
			}
			
			if (! BString (ip->Text ()).CountChars ()) {
				BNetAddress auto_detect (host->Text ());
				if (auto_detect.InitCheck () != B_OK) {
					(new BAlert ("!", "Could not auto-detect the host's IP address, please enter it manually.", "Ok"))->Go ();
				} else {
				}
				break;
			}

			BMessage * mount_msg;
			if (index == -1) {
				mount_msg = new BMessage (MOUNT_ADDED);
			} else {
				mount_msg = new BMessage (MOUNT_EDITED);
				mount_msg->AddInt32 ("index", index);
			}
			mount_msg->AddString ("ip", ip->Text ());
			mount_msg->AddString ("workgroup", workgroup->Text ());
			mount_msg->AddString ("host", host->Text ());
			mount_msg->AddString ("share", share->Text ());
			mount_msg->AddString ("user", user->Text ());
			mount_msg->AddString ("pass", pass->Text ());
			mount_msg->AddString ("mount", mount->Text ());

			(SelectionWindow *) select_win->PostMessage (mount_msg);

			Unlock ();

			Quit ();
			break;
		} default : {
			BWindow::MessageReceived (msg);
			break;
		}
	}
}

bool EditMountWindow::QuitRequested (void) {
	return true;
}