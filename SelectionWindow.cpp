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

#include <fstream>
#include <iostream>

#include <Alert.h>
#include <Application.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <Roster.h>
#include <ScrollView.h>
#include <String.h>

#include "constants.h"
#include "EditMountWindow.h"
#include "MountItem.h"
#include "MountListView.h"
#include "SelectionWindow.h"

using namespace std;

/*
 * SelectionWindow::SelectionWindow
 *
 * SelectionWindow's constructor
 */

SelectionWindow::SelectionWindow (BRect frame, SettingsFile * s)
: BWindow (frame, "Mount Selector", B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS),
  settings (s) {
	// set the size limits

	float temp, max_width, max_height;

	GetSizeLimits (& temp, & max_width, & temp, & max_height);
	SetSizeLimits (SELECTION_WINDOW_MIN_W, max_width, SELECTION_WINDOW_MIN_H, max_height);

	// create the menu

	BMenu * menu = new BMenu ("File");

	BMenuItem * pref_item = new BMenuItem ("Preferences...", new BMessage (MENU_FILE_PREFERENCES));
	pref_item->SetEnabled (false);

	menu->AddItem (pref_item);

	menu->AddSeparatorItem ();

	menu->AddItem (new BMenuItem ("Quit", new BMessage (B_QUIT_REQUESTED), 'Q'));

	BMenuBar * menu_bar = new BMenuBar (Bounds (), "menu_bar");

	menu_bar->AddItem (menu);

	menu = new BMenu ("Help");

	menu->AddItem (new BMenuItem ("Help...", new BMessage (MENU_HELP_HELP)));

	menu->AddItem (new BMenuItem ("About...", new BMessage (B_ABOUT_REQUESTED)));

	menu_bar->AddItem (menu);

	AddChild (menu_bar);

	// create the list of mounts

	BRect list_frame = Bounds ();
	list_frame.left += 5;
	list_frame.top = menu_bar->Frame ().bottom + 5;
	list_frame.right -= 5 + B_V_SCROLL_BAR_WIDTH;
	list_frame.bottom -= 40 + B_H_SCROLL_BAR_HEIGHT;

	mounts_list = new MountListView (list_frame);

	AddChild (new BScrollView ("mounts_list_scroll_view", mounts_list, B_FOLLOW_ALL, 0, true, true));

	// create the buttons

	BRect button_frame = Bounds ();
	button_frame.left = button_frame.right - 95;
	button_frame.top = button_frame.bottom - 35;
	button_frame.right -= 5;
	button_frame.bottom -= 5;

	AddChild (delete_button = new BButton (button_frame, "delete_mount_button", "Delete", new BMessage (DELETE_MOUNT), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM));
	delete_button->SetEnabled (false);

	button_frame.right = button_frame.right - 90;
	button_frame.left = button_frame.left - 90;

	AddChild (edit_button = new BButton (button_frame, "edit_mount_button", "Edit", new BMessage (EDIT_MOUNT), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM));
	edit_button->SetEnabled (false);

	button_frame.right = button_frame.right - 90;
	button_frame.left = button_frame.left - 90;

	AddChild (new BButton (button_frame, "add_mount_button", "Add", new BMessage (ADD_MOUNT), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM));

	button_frame = Bounds ();
	button_frame.left += 5;
	button_frame.bottom -= 5;
	button_frame.right = button_frame.left + 90;
	button_frame.top = button_frame.bottom - 30;

	AddChild (mount_button = new BButton (button_frame, "mount_mutton", "Mount", new BMessage (MOUNT), B_FOLLOW_LEFT | B_FOLLOW_BOTTOM));
	mount_button->SetEnabled (false);

	// load all the mounts

	LoadMounts ();
}

/*
 * SelectionWindow::FrameMoved
 *
 * 
 */

void SelectionWindow::FrameMoved (BPoint) {
	UpdateSettingsFrame ();
}

/*
 * SelectionWindow::FrameResized
 *
 * if the frame is resized, it updates the related info in
 * the settings file
 */

void SelectionWindow::FrameResized (float, float) {
	UpdateSettingsFrame ();
}

/*
 * SelectionWindow::MessageReceived
 *
 * handles messages
 */

void SelectionWindow::MessageReceived (BMessage * msg) {
	const char * ip, * workgroup, * host, * share, * user, * pass, * mount;
	int32 index;

	switch (msg->what) {
		case MENU_FILE_PREFERENCES : {
			// Display the preferences
			break;
		} case MENU_HELP_HELP : {
			HelpRequested ();
			break;
		} case B_ABOUT_REQUESTED : {
			BAlert * about_box = new BAlert ("About EasyMounter", "EasyMounter 0.4\n© 2001, Matt Bishop (mrbish@excite.com)", "Ok");
			about_box->Go ();
			break;
		} case SELECTION_CHANGED : {
			bool selected;
			msg->FindBool ("selected", & selected);
			if (selected) {
				SetButtonsEnabled (true);
			} else {
				SetButtonsEnabled (false);
			}

			break;
		} case ADD_MOUNT : {
			EditMountWindow * emw = new EditMountWindow (this, Frame ());
			emw->Show ();
			break;
		} case EDIT_MOUNT : {
			Lock ();

			MountItem * item = (MountItem *) mounts_list->ItemAt (mounts_list->CurrentSelection ());
			EditMountWindow * emw = new EditMountWindow (this, Frame (), mounts_list->CurrentSelection (), item->GetIP (), item->GetWorkgroup (), item->GetHost (), item->GetShare (), item->GetUser (), item->GetPass (), item->GetMount ());
			emw->Show ();

			Unlock ();
			break;
		} case DELETE_MOUNT : {
			Lock ();
			DeleteMount (mounts_list->CurrentSelection ());
			Unlock ();
			break;
		} case MOUNT_ADDED : {
			ip = msg->FindString ("ip");
			workgroup = msg->FindString ("workgroup");
			host = msg->FindString ("host");
			share = msg->FindString ("share");
			user = msg->FindString ("user");
			pass = msg->FindString ("pass");
			mount = msg->FindString ("mount");

			AddMount (new MountItem (ip, workgroup, host, share, user, pass, mount));
			SetButtonsEnabled (false);

			break;
		} case MOUNT_EDITED : {
			Lock ();

			msg->FindInt32 ("index", & index);
			ip = msg->FindString ("ip");
			workgroup = msg->FindString ("workgroup");
			host = msg->FindString ("host");
			share = msg->FindString ("share");
			user = msg->FindString ("user");
			pass = msg->FindString ("pass");
			mount = msg->FindString ("mount");

			EditMount (index, new MountItem (ip, workgroup, host, share, user, pass, mount));
			SetButtonsEnabled (false);

			Unlock ();
			break;
		} case MOUNT : {
			Lock ();
			Mount (mounts_list->CurrentSelection ());
			Unlock ();
			break;
		} default : {
			BWindow::MessageReceived (msg);
			break;
		}
	}
}

/*
 * SelectionWindow::SetButtonsEnabled
 *
 * sets whether the "mount", "edit" and "delete" buttons are enabled
 */

void SelectionWindow::SetButtonsEnabled (bool enabled) {
	mount_button->SetEnabled (enabled);
	edit_button->SetEnabled (enabled);
	delete_button->SetEnabled (enabled);
}

/*
 * SelectionWindow::AddMount
 *
 * adds a mount to the list
 */

void SelectionWindow::AddMount (MountItem * item) {
	Lock ();

	mounts_list->AddItem (item);
	UpdateSettingsMounts (mounts_list->CountItems () - 1);

	Unlock ();
}

/*
 * SelectionWindow::EditMount
 *
 * edits a mount in the list
 */

void SelectionWindow::EditMount (int32 index, MountItem * item) {
	Lock ();

	mounts_list->ReplaceItem (index, item);
	UpdateSettingsMounts (index);

	Unlock ();
}

/*
 * SelectionWindow::DeleteMount
 *
 * deletes a mount from the list
 */

void SelectionWindow::DeleteMount (int32 index) {
	Lock ();

	mounts_list->RemoveItem (index);
	UpdateSettingsMounts (index, true);

	Unlock ();
}

/*
 * SelectionWindow::Mount
 *
 * mounts a mount item in the list
 */

bool SelectionWindow::Mount (int32 index) {
	bool r = true;
	BString cmd;
	MountItem * item;

	Lock ();

	item = (MountItem *) mounts_list->ItemAt (index);

	cmd << "mkdir " << item->GetMount () << "\n";
	cmd << "cifsmount -I " << item->GetIP () << " -W " << item->GetWorkgroup () << " \\\\\\\\" << item->GetHost () << "\\\\" << item->GetShare () << " " << item->GetUser () << " " << item->GetPass () << " " << item->GetMount () << " > /tmp/f.txt\n";
	system (cmd.String ());

	BString s;
	ifstream f ("/tmp/f.txt");
	while (f) {
		s << (char) f.get ();
	}
	f.close ();
	s.Truncate (s.CountChars () - 2, false);

	BAlert * cifsmount_output = new BAlert ("cifsmount output", s.String (), "Ok");
	cifsmount_output->Go ();

	Unlock ();

	return r;
}

/*
 * SelectionWindow::LoadMounts
 *
 * loads all the mounts from the settings file
 */

void SelectionWindow::LoadMounts (void) {
	int32 count;

	Lock ();

	if (settings->FindInt32 ("count", & count) == B_OK) {
		string ip, workgroup, host, share, user, pass, mount;

		for (int32 i = 0; i < count; ++ i) {
			ip = settings->FindString ("ip", i);
			workgroup = settings->FindString ("workgroup", i);
			host = settings->FindString ("host", i);
			share = settings->FindString ("share", i);
			user = settings->FindString ("user", i);
			pass = settings->FindString ("pass", i);
			mount = settings->FindString ("mount", i);

			mounts_list->AddItem (new MountItem (ip.c_str (), workgroup.c_str (), host.c_str (), share.c_str (), user.c_str (), pass.c_str (), mount.c_str ()));
		}
	}

	Unlock ();
}

/*
 * SelectionWindow::Save
 *
 * saves all the information to the settings file
 */

void SelectionWindow::Save (void) {
//	UpdateSettingsMounts ();
	UpdateSettingsFrame ();

	settings->Save ();
}

/*
 * SelectionWindow::HelpRequested
 *
 * displays the help file
 */

void SelectionWindow::HelpRequested (void) {
	app_info appinfo;
	be_app->GetAppInfo (& appinfo);

	BEntry app_entry (& appinfo.ref);
	BPath app_path;
	app_entry.GetPath (& app_path);
	app_path.GetParent (& app_path);

	BString cmd;
	cmd << "/system/Tracker " << app_path.Path () << "/Readme.html &\n";

	system (cmd.String ());
}

/*
 * SelectionWindow::QuitRequested
 *
 * saves the settings, and tells the app to quit,
 * thens gives the ok to close the window
 */

bool SelectionWindow::QuitRequested (void) {
	Save ();

	be_app->PostMessage (B_QUIT_REQUESTED, be_app);
	return true;
}

/*
 * SelectionWindow::UpdateSettingsMounts
 *
 * updates the mounts in the settings file
 */

void SelectionWindow::UpdateSettingsMounts (int32 index = -1, bool update_successors = false) {
	Lock ();

	int32 count = mounts_list->CountItems ();

	if (settings->ReplaceInt32 ("count", count) < B_OK) {
		settings->AddInt32 ("count", count);
	}

	int32 i;
	MountItem * item;

	if (index > -1 && ! update_successors) {
		item = (MountItem *) mounts_list->ItemAt (index);

		if (settings->ReplaceString ("ip", index, item->GetIP ()) < B_OK) {
			settings->AddString ("ip", item->GetIP ());
		}
		if (settings->ReplaceString ("workgroup", index, item->GetWorkgroup ()) < B_OK) {
			settings->AddString ("workgroup", item->GetWorkgroup ());
		}
		if (settings->ReplaceString ("host", index, item->GetHost ()) < B_OK) {
			settings->AddString ("host", item->GetHost ());
		}
		if (settings->ReplaceString ("share", index, item->GetShare ()) < B_OK) {
			settings->AddString ("share", item->GetShare ());
		}
		if (settings->ReplaceString ("user", index, item->GetUser ()) < B_OK) {
			settings->AddString ("user", item->GetUser ());
		}
		if (settings->ReplaceString ("pass", index, item->GetPass ()) < B_OK) {
			settings->AddString ("pass", item->GetPass ());
		}
		if (settings->ReplaceString ("mount", index, item->GetMount ()) < B_OK) {
			settings->AddString ("mount", item->GetMount ());
		}
	} else if (index > -1 && update_successors) {
		for (i = index; i < count; ++ i) {
			item = (MountItem *) mounts_list->ItemAt (i);
	
			settings->ReplaceString ("ip", i, item->GetIP ());
			settings->ReplaceString ("workgroup", i, item->GetWorkgroup ());
			settings->ReplaceString ("host", i, item->GetHost ());
			settings->ReplaceString ("share", i, item->GetShare ());
			settings->ReplaceString ("user", i, item->GetUser ());
			settings->ReplaceString ("pass", i, item->GetPass ());
			settings->ReplaceString ("mount", i, item->GetMount ());
		}
		settings->RemoveData ("ip", count);
		settings->RemoveData ("workgroup", count);
		settings->RemoveData ("host", count);
		settings->RemoveData ("share", count);
		settings->RemoveData ("user", count);
		settings->RemoveData ("pass", count);
		settings->RemoveData ("mount", count);
	} else {
		for (i = 0; i < count; ++ i) {
			item = (MountItem *) mounts_list->ItemAt (i);
	
			settings->ReplaceString ("ip", i, item->GetIP ());
			settings->ReplaceString ("workgroup", i, item->GetWorkgroup ());
			settings->ReplaceString ("host", i, item->GetHost ());
			settings->ReplaceString ("share", i, item->GetShare ());
			settings->ReplaceString ("user", i, item->GetUser ());
			settings->ReplaceString ("pass", i, item->GetPass ());
			settings->ReplaceString ("mount", i, item->GetMount ());
		}
	}

	Unlock ();
}

/*
 * SelectionWindow::UpdateSettingsFrame
 *
 * updates the selection window's frame in the settings file
 */

void SelectionWindow::UpdateSettingsFrame (void) {
	if (settings->ReplaceRect ("select_win_frame", Frame ()) < B_OK) {
		settings->AddRect ("select_win_frame", Frame ());
	}
}