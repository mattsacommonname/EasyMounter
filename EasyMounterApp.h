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

#ifndef EASY_MOUNTER_APP_H
#define EASY_MOUNTER_APP_H

#include <Application.h>

#include "SelectionWindow.h"
#include "SettingsFile.h"

class EasyMounterApp
: public BApplication {
	public:
		EasyMounterApp (void);
		~EasyMounterApp (void);
		virtual void MessageReceived (BMessage *);
	private:
		SelectionWindow * select_win;
		SettingsFile * settings;
};

#endif