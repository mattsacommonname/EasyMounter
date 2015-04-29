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

#ifndef MOUNT_ITEM_H
#define MOUNT_ITEM_H

#include <string>

#include <ListItem.h>

using namespace std;

class MountItem
: public BStringItem {
	public:
		MountItem (const char *, const char *, const char *, const char *, const char *, const char *, const char *);
		void Edit (const char *, const char *, const char *, const char *, const char *, const char *, const char *);
		bool IsMounted (void);
		void SetMounted (bool);
		const char * GetIP (void);
		const char * GetWorkgroup (void);
		const char * GetHost (void);
		const char * GetShare (void);
		const char * GetUser (void);
		const char * GetPass (void);
		const char * GetMount (void);
	private:
		string ip, workgroup, host, share, user, pass, mount;
		bool mounted;
};

#endif