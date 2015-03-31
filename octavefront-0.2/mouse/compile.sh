function compile ()
{
    if [ $make_clean = yes ] ; then
	if [ -f Makefile ] ; then
	    run_prg make maintainer-clean
	    rm Makefile
	fi
	if [ -f configure ] ; then
	    rm configure
	fi
	find . -name "*.[dao]" -print0 | xargs -0 rm
    fi

    if [ ! -f configure ] ; then
	run_prg autogen.sh
    fi

    if [ ! -f Makefile ] ; then
	run_prg ./configure --prefix=$MOUSE_TOP/install \
	    --enable-rna=$RNA_HOME --enable-aterm \
	    --disable-dl --disable-shl --enable-static \
	    --enable-readline --without-fastblas
    fi

    CFLAGS="$CDEBUG" \
    CXXFLAGS="$CXXDEBUG" \
    FFLAGS="$FDEBUG" \
    LDFLAGS="$LDDEBUG" \
    run_prg make
    
    if [ ! -x $MOUSE_TOP/install/bin/octave -o $make_clean = yes ] ; then
	mkdir_if_necessary $MOUSE_TOP/install
	run_prg make install
    fi
}
