
import virtualenv, textwrap

def mine_versions(eggs):
    f = open('python-env-requirements.txt', 'rt')
    versions = {}
    for line in f.readlines():
        line = line[:-1]
        for egg in eggs:
            if line.startswith(egg):
                versions[egg] = line
    return versions

def main():
    deps = ['pip', 'virtualenv', 'fabric', 'bzr', 'pycrypto', 'paramiko']
    deps = ', '.join(["'{0}'".format(x) for x in mine_versions(deps).values()])

    output = virtualenv.create_bootstrap_script(textwrap.dedent("""
    import os, sys, subprocess

    def extend_parser(parser):
        for opt in ['clear','no-site-packages','unzip-setuptools','relocatable','distribute','setuptools']:
            parser.remove_option('--'+opt)

        parser.add_option('--bbcom-branch', dest='bbcom_branch', metavar='BRANCH',
            help='Location of the bbcom_branch',
            default='bzr+ssh://code.hackyourlife.org/home/dsnopek/bzr-lingwo/lingwoorg/refactor-1')

        parser.add_option('--only-virtualenv', dest='only_virtualenv', action='store_true',
            help='Only create the virtualenv, not the entire BiblioBird project directory',
            default=False)

    def adjust_options(options, args):
        if len(args) > 0:
            if os.path.exists(args[0]):
                print >> sys.stderr, "ERROR: %s already exists!" % args[0]
                sys.exit(1)
            if not options.only_virtualenv:
                os.mkdir(args[0])
                options.bbcom_project_dir = args[0]
                args[0] = join(args[0], 'python-env')

        options.no_site_packages = True
        options.prompt = 'bibliobird'

    def after_install(options, home_dir):
        subprocess.call([join(home_dir, 'bin', 'pip'),
            'install', '-U', {deps}])
        if not options.only_virtualenv:
            subprocess.call([join(home_dir, 'bin', 'bzr'),
                'co', options.bbcom_branch,
                join(options.bbcom_project_dir, 'bbcom')])
    """.format(deps=deps)))

    f = open('bootstrap.py', 'w')
    f.write(output)
    f.close()

if __name__ == '__main__': main()
