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

#include "MountItem.h"

MountItem::MountItem (const char * i, const char * w, const char * h, const char * s, const char * u, const char * p, const char * m)
: BStringItem ("New Mount"), ip (i), workgroup (w), host (h), share (s), user (u), pass (p), mount (m) {
	string t = "\\\\";
	t += host;
	t += "\\";
	t += share;
	t += " => ";
	t += mount;

	SetText (t.c_str ());
}

void MountItem::Edit (const char * i, const char * w, const char * h, const char * s, const char * u, const char * p, const char * m) {
	ip = i;
	workgroup = w;
	host = h;
	share = s;
	user = u;
	pass = p;
	mount = m;

	string t = "\\\\";
	t += host;
	t += "\\";
	t += share;
	t += " => ";
	t += mount;

	SetText (t.c_str ());
}

bool MountItem::IsMounted (void) {
	return mounted;
}

void MountItem::SetMounted (bool m) {
	mounted = m;
}

const char * MountItem::GetIP (void) {
	return ip.c_str ();
}

const char * MountItem::GetWorkgroup (void) {
	return workgroup.c_str ();
}

const char * MountItem::GetHost (void) {
	return host.c_str ();
}

const char * MountItem::GetShare (void) {
	return share.c_str ();
}

const char * MountItem::GetUser (void) {
	return user.c_str ();
}

const char * MountItem::GetPass (void) {
	return pass.c_str ();
}

const char * MountItem::GetMount (void) {
	return mount.c_str ();
}