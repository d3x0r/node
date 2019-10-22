# Copyright (c) 2009 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'variables': {
    'use_system_sexpat%': 0
  },
  'conditions': [
    ['use_system_sexpat==0', {
      'targets': [
        {
          'target_name': 'sexpat',
          'type': 'static_library',
          'sources': [
          
 'xmlparse.c',
'xmlrole.c',
'xmltok.c',
'xmltok_impl.c',
'xmltok_ns.c',

          ],
          'include_dirs': [
            '.',
          ],
          'direct_dependent_settings': {
            'include_dirs': [
              '.',
            ],
          },
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
    }, {
      'targets': [
        {
          'target_name': 'sexpat',
          'type': 'static_library',
          'direct_dependent_settings': {
            'defines': [
              'USE_SYSTEM_EXPAT',
            ],
          },
          'defines': [
            'USE_SYSTEM_EXPAT',
          ],
          'link_settings': {
            'libraries': [
              '-lexpat',
            ],
          },
        },
      ],
    }],
  ],
}
