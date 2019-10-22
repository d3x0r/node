# Copyright (c) 2009 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
      'targets': [
        {
          'target_name': 'sack',
          'type': 'static_library',
          'sources': [
          
                    'sack.cc', 'sack_psi.cc', 'sack_imglib.cc', 'sack_imglib_puregl2.cc', 'sack_vidlib.cc', 'sack_vidlib_puregl2.cc', 'sack.wrap.cc', 'sqlite3.c'

          ],
          'include_dirs': [
            '.',
            '../openssl/openssl/include',
          ],
          'direct_dependent_settings': {
            'include_dirs': [
              '.',
            ],
          },
	  'defines': ['SQLITE_ENABLE_COLUMN_METADATA','HAVE_SSL','STATIC_RENDER_INTERFACE'],
          'conditions': [
            ['OS!="win"', {
              'cflags!': [  ],
              'defines': [  ],
            }],
            ['OS=="mac" or OS=="ios" or OS=="freebsd" or OS=="android"', {
              # Mac, Android and the BSDs don't have fopen64, ftello64, or
              # fseeko64. We use fopen, ftell, and fseek instead on these
              # systems.
              'defines': [
                
              ],
            }],
          ],
        },
      ],
    
}
