# Copyright (c) 2009 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'variables': {
    'use_system_png%': 0
  },
  'conditions': [
    ['use_system_png==0', {
      'targets': [
        {
          'target_name': 'png',
          'type': 'static_library',
          'sources': [
          
                    'png.c', 'pngset.c','pngget.c', 'pngrutil.c','pngtrans.c', 'pngwutil.c','pngmem.c', 'pngpread.c',
                    'pngread.c',  'pngerror.c',  'pngwrite.c',
		  'pngrtran.c',  'pngwtran.c',  'pngrio.c',  'pngwio.c',


          ],
          'include_dirs': [
            '../zlib',
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
          'target_name': 'png',
          'type': 'static_library',
          'direct_dependent_settings': {
            'defines': [
              'USE_SYSTEM_JPEG',
            ],
          },
          'defines': [
            'USE_SYSTEM_JPEG',
          ],
          'link_settings': {
            'libraries': [
              '-lpng',
            ],
          },
        },
      ],
    }],
  ],
}
