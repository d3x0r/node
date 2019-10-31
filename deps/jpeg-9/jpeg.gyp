# Copyright (c) 2009 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'variables': {
    'use_system_jpeg%': 0
  },
  'conditions': [
    ['use_system_jpeg==0', {
      'targets': [
        {
          'target_name': 'jpeg',
          'type': 'static_library',
          'sources': [
          
          

'jmemnobs.c',
'jaricom.c', 'jcomapi.c', 'jutils.c', 'jerror.c', 'jmemmgr.c',

'jcarith.c', 'jcapimin.c', 'jcapistd.c', 'jctrans.c', 'jcparam.c', 'jdatadst.c',
        'jcinit.c', 'jcmaster.c', 'jcmarker.c', 'jcmainct.c', 'jcprepct.c',
        'jccoefct.c', 'jccolor.c', 'jcsample.c', 'jchuff.c',
        'jcdctmgr.c', 'jfdctfst.c', 'jfdctflt.c', 'jfdctint.c',

        'jdarith.c', 'jdapimin.c', 'jdapistd.c', 'jdtrans.c', 'jdatasrc.c',
        'jdmaster.c', 'jdinput.c', 'jdmarker.c', 'jdhuff.c',
        'jdmainct.c', 'jdcoefct.c', 'jdpostct.c', 'jddctmgr.c', 'jidctfst.c',
        'jidctflt.c', 'jidctint.c', 'jdsample.c', 'jdcolor.c',
        'jquant1.c', 'jquant2.c', 'jdmerge.c',


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
          'target_name': 'jpeg',
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
              '-ljpeg-9',
            ],
          },
        },
      ],
    }],
  ],
}
