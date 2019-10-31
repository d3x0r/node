{
  'targets': [
    {
      'target_name': 'freetype',
      'type': 'static_library',
      'sources': [
        'src/base/ftbbox.c',
        'src/base/ftbitmap.c',
        'src/base/ftglyph.c',
        'src/base/ftlcdfil.c',
        'src/base/ftstroke.c',
        #'src/base/ftxf86.c',
        'src/base/ftbase.c',
        'src/base/ftsystem.c',
        'src/base/ftinit.c',
        'src/base/ftgasp.c',
        'src/base/ftfstype.c',
        'src/raster/raster.c',
        'src/sfnt/sfnt.c',
        'src/smooth/smooth.c',
        'src/autofit/autofit.c',
        'src/truetype/truetype.c',
        'src/cff/cff.c',
        'src/psnames/psnames.c',
        'src/pshinter/pshinter.c',

# added for linker
        'src/lzw/ftlzw.c',
        'src/gzip/ftgzip.c',
        'src/cid/type1cid.c',
        'src/bdf/bdf.c',
        'src/psaux/psaux.c',
        'src/pcf/pcf.c',
        'src/pfr/pfr.c',
        'src/type1/type1.c',
        'src/type42/type42.c',
        'src/winfonts/winfnt.c',
      ],
      'include_dirs': [
        'include',
      ],
      'defines': [
      	'FT2_BUILD_LIBRARY'
      ],
      'cflags': [
        '-W',
        '-Wall',
        '-fPIC',
        '-DPIC',
        '-DDARWIN_NO_CARBON',
        '-DFT2_BUILD_LIBRARY',
      ],
      'direct_dependent_settings': {
        'include_dirs': [
          'include',  # For ft2build.h
        ],
      },
    },
  ],
}

# Local Variables:
# tab-width:2
# indent-tabs-mode:nil
# End:
# vim: set expandtab tabstop=2 shiftwidth=2:
