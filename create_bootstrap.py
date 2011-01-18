
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
    deps = ', '.join(["'{0}'".format(x) for x in mine_versions(['pip', 'virtualenv', 'fabric', 'bzr']).values()])

    output = virtualenv.create_bootstrap_script(textwrap.dedent("""
    import os, subprocess

    def extend_parser(optparse_parser):
        print optparse_parser

    def after_install(options, home_dir):
        subprocess.call([join(home_dir, 'bin', 'pip'),
            'install', '-U', {deps}])
        # TODO: maybe install http://samba.org/~jelmer/bzr/bzr-git-0.5.2.tar.gz ??
        subprocess.call([join(home_dir, 'bin', 'bzr'),
            'co', 'bzr+ssh://code.hackyourlife.org/home/dsnopek/bzr-lingwo/lingwoorg/refactor-1', 'bbcom'])
    """.format(deps=deps)))

    f = open('bootstrap.py', 'w')
    f.write(output)
    f.close()

if __name__ == '__main__': main()
