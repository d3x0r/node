# Copyright (c) 2009 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
      'targets': [
        {
          'target_name': 'glew',
          'type': 'static_library',
          'sources': [
          
                    'src/glew.c'

          ],
          'include_dirs': [
            'include',
          ],
          'direct_dependent_settings': {
            'include_dirs': [
              'include',
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
    
}
