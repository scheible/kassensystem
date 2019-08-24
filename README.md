# kassensystem
A private highly customized POS software for the use in beverage selling in Germany

# compilation steps for linux
1. Install Qt:

sudo apt-get install build-essential
sudo apt-get install qtcreator
sudo apt-get install qt5-default

[https://stackoverflow.com/questions/48147356/install-qt-on-ubuntu/48147619]

Additionally install this for the excel support to work:

sudo apt-get install qtbase5-private-dev

[https://github.com/dbzhang800/QtXlsxWriter/issues/9]

with this, you should be able to open the project, configure it and compile it
