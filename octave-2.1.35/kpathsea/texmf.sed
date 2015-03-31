s%@prefix@%/usr/local%g
s%@exec_prefix@%/usr/local%g
s%@bindir@%/usr/local/bin%g
s%@scriptdir@%/usr/local/bin%g
s%@libdir@%/usr/local/lib%g
s%@datadir@%/usr/local/share%g
s%@infodir@%/usr/local/info%g
s%@includedir@%/usr/local/include%g
s%@manext@%1%g
s%@mandir@%/usr/local/man/man1%g
s%@texmf@%/usr/local/lib/texmf%g
s%@web2cdir@%/usr/local/lib/texmf/web2c%g
s%@vartexfonts@%/var/tmp/texfonts%g
s%@texinputdir@%/usr/local/lib/texmf/tex%g
s%@mfinputdir@%/usr/local/lib/texmf/metafont%g
s%@mpinputdir@%/usr/local/lib/texmf/metapost%g
s%@fontdir@%/usr/local/lib/texmf/fonts%g
s%@fmtdir@%/usr/local/lib/texmf/web2c%g
s%@basedir@%/usr/local/lib/texmf/web2c%g
s%@memdir@%/usr/local/lib/texmf/web2c%g
s%@texpooldir@%/usr/local/lib/texmf/web2c%g
s%@mfpooldir@%/usr/local/lib/texmf/web2c%g
s%@mppooldir@%/usr/local/lib/texmf/web2c%g
s%@dvips_plain_macrodir@%/usr/local/lib/texmf/tex/plain/dvips%g
s%@dvilj_latex2e_macrodir@%/usr/local/lib/texmf/tex/latex/dvilj%g
s%@dvipsdir@%/usr/local/lib/texmf/dvips%g
s%@psheaderdir@%/usr/local/lib/texmf/dvips%g
s%@default_texsizes@%300:600%g
s%/usr/local/lib/texmf%\$TEXMF%g
/^ *TEXMFMAIN[ =]/s%\$TEXMF%/usr/local/lib/texmf%
/^[% ]*TEXMFLOCAL[ =]/s%\$TEXMF%/usr/local/lib/texmf%
/^ *TEXMFCNF[ =]/s%@web2c@%/usr/local/lib/texmf/web2c%
