
from setuptools import setup

setup(
    name="BibliobirdAnkiServer",
    version="2.0",
    description="An AnkiServer for Bibliobird.com",
    author="David Snopek",
    author_email="dsnopek@gmail.com",
    install_requires=["PasteDeploy>=1.3.2","AnkiServer>=2.0.0a1"],
    entry_points="""
    [paste.app_factory]
    sync_app = BibliobirdAnkiServer.sync_app:make_app
    rest_app = BibliobirdAnkiServer.rest_app:make_app
    """,
)

