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

#include <Window.h>

#include "constants.h"
#include "MountListView.h"

MountListView::MountListView (BRect frame)
: BListView (frame, "mounts_list_view", B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL) {
}

void MountListView::SelectionChanged (void) {
	BMessage * msg = new BMessage (SELECTION_CHANGED);

	if (CurrentSelection () > -1) {
		msg->AddBool ("selected", true);
	} else {
		msg->AddBool ("selected", false);
	}

	Window ()->PostMessage (msg);
}