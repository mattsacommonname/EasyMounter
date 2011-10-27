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