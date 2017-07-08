
import os
import sys
import glob
import argparse


__version__ = '1.4.3'
__library_name__ = 'libNGL.a'

# ------------------------------------------------------------------------------
# error message
# ------------------------------------------------------------------------------
def error(msg):
    """ Display an error message and terminate.  msg is the text of the error
    message.
    """
    msg = str(msg)
    sys.stderr.write("Error: %s\n" % msg)
    sys.exit(True)

# ------------------------------------------------------------------------------
# inform message
# ------------------------------------------------------------------------------
def inform(msg):
    """ Display an information message.  msg is the text of the error message.
    """
    msg = str(msg)
    sys.stdout.write("Inform: %s\n" % msg)

# ------------------------------------------------------------------------------
# newline
# ------------------------------------------------------------------------------
def newline():
    sys.stdout.write("\n")


# ------------------------------------------------------------------------------
# objects_dir
# ------------------------------------------------------------------------------
def objects_dir():
    return '.\\obj'

# ------------------------------------------------------------------------------
# create_objectsfiles_dir
# ------------------------------------------------------------------------------
def create_objectsfiles_dir():

    _dir = os.path.abspath(objects_dir())

    # create dir for object files or clean
    if not os.path.exists(_dir):
        inform('Create object dir %s ...' % _dir)
        os.makedirs(_dir, mode=0x777)
    else:
        inform('Clean object dir %s ...' % _dir)
        for f in glob.glob(_dir + '\\*.*'):
            os.remove(f)

# ------------------------------------------------------------------------------
# build gcc compiler options
# ------------------------------------------------------------------------------
def build_otions():

    return [
        '-mcpu=cortex-m4',
        '-mfpu=fpv4-sp-d16',
        '-mfloat-abi=hard',
        '-mthumb',
        '-Wall',
        '-ffunction-sections',
        '-g',
        '-O3',
        '-c',
        '-D__FPU_USED', ]

# ------------------------------------------------------------------------------
# includes
# ------------------------------------------------------------------------------
def includes():

    include_dirs = [
        '.',
        '.\\HAL_MAL',
        '.\\Bitmaps',
        '.\\Colors',
        '.\\Primitives',
        '.\\Font',
        '.\\GUI',
        '.\\TouchScreen']

    return ['-I%s' % os.path.abspath(inc) for inc in include_dirs]

# ------------------------------------------------------------------------------
# sources
# ------------------------------------------------------------------------------
def sources():

    files = [

        '.\\HAL_MAL\\HX8352\\HX8352.c',
        '.\\HAL_MAL\\SSD1289\\SSD1289.c',
        '.\\HAL_MAL\\R61581\\R61581.c',

        '.\\HAL_MAL\\LCD_GPIO.c',
        '.\\HAL_MAL\\LCD_HAL.c',
        '.\\HAL_MAL\\LCD_MAL.c',
        '.\\HAL_MAL\\MAL.c',

        '.\\Colors\\colors.c',

        '.\\Font\\Font.c',

        '.\\Bitmaps\\bitmap.c',
        '.\\Bitmaps\\picojpeg.c',

        '.\\Primitives\\Graphics_Primitive.c',
        '.\\Primitives\\traingle.c',
        '.\\Primitives\\clip.c',

        '.\\GUI\\page.c',
        '.\\GUI\\Objects\\button.c',
        '.\\GUI\\Objects\\label.c',
        '.\\GUI\\Objects\\greed.c',
        '.\\GUI\\Objects\\fillbar.c',
        '.\\GUI\\Objects\\graphscale.c',
        '.\\GUI\\Objects\\seekbar.c',
        '.\\GUI\\Objects\\checkbox.c',

        '.\\TouchScreen\\NGL_Touch.c',
        '.\\TouchScreen\\NGL_Touch_Events.c']

    return [os.path.abspath(f) for f in files]

# ------------------------------------------------------------------------------
# build
# ------------------------------------------------------------------------------
def build(verbose):

    cmd = 'arm-none-eabi-gcc ' + \
       ' '.join(build_otions()) + ' ' + \
       ' '.join(includes()) + ' ' + \
       ' '.join(sources())

    # create and change dir to objects destination
    create_objectsfiles_dir()
    os.chdir( objects_dir() )

    inform('Starting build...\nOut path - %s' % os.path.abspath('.\\') )
    if verbose:
        inform(cmd)

    # run and return operation status
    return os.system(cmd)

# ------------------------------------------------------------------------------
# link
# ------------------------------------------------------------------------------
def link(out_path, verbose):

    filelist = [ os.path.abspath(f) for f in glob.glob('.\\*.*') ]

    cmd = 'arm-none-eabi-ar rvs ' + \
           __library_name__ + ' ' + \
           ' '.join(filelist)

    # change dir to dest out dir
    os.chdir(out_path)

    inform('Start link...\nOut path - %s' % os.path.abspath('.\\') )
    if verbose:
        inform(cmd)

    # run and return operation status
    return os.system(cmd)

# ------------------------------------------------------------------------------
# create cmd line arguments parser
# ------------------------------------------------------------------------------
def createArgParser():
    _prog = "ngluic"
    desctext = 'Builder script for NGL library, v%s' % __version__
    epitext = 'Find more info visit http://hobby-research.at.ua ; https://github.com/LeftRadio/ngl'

    parser = argparse.ArgumentParser( description = desctext,
                                      epilog = epitext,
                                      prog = _prog )

    parser.add_argument ( '-V', '--version', action='version', help = 'version',
                            version = __version__ )

    parser.add_argument ( '-o', '--outpath', dest = 'out_path', type = str,
                            default = os.path.dirname(__file__)+'\\',
                            metavar = '[Path]',
                            help = 'Path for output *.a library file' )

    parser.add_argument( '-v', '--verbose', action='store_true', default = True,
                            help = ('increase output verbosity '
                                    '[default: True]') )
    return parser

# ------------------------------------------------------------------------------
# main
# ------------------------------------------------------------------------------
def main():

    parser = createArgParser()
    args = parser.parse_args()

    # if no args
    if not len(sys.argv[1:]):
        parser.print_help()
    newline()

    # strart build
    if build(args.verbose) != 0:
        error('Build failed!')

    # start link
    if link(args.out_path, args.verbose) != 0:
        error('Link failed!')

    # final
    newline()
    inform( '{0} SUCCESS {0}'.format('-' * 8) )
    inform( 'Library file path - %s%s' % (args.out_path, __library_name__) )


# ------------------------------------------------------------------------------
# program start here
# ------------------------------------------------------------------------------
if __name__ == '__main__':
    sys.exit(main())
