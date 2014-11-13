To run the audio demo:

1) Load 1.12 firmware
2) Load the file 'multiwaves.tbin' (loads in banks 48-4F)
3) Set up ip address
4) Create directory 'html'
5) Put the following files in directory 'html':
      ds80c400.gif
      logo.gif
      micro4.jpg
      index.html
6) Put in root 'server.tini'
7) Run 'java server.tini'
8) Connect to ip address with browser

Files in this archive:
  audioplayer.a51       - assembly source with DAC wiggling and timer interrupt code
  ds80c400.gif          - cute little web page image
  HTTPAudioServer       - Starts of the HTTP server process
  index.html            - Default page for server with form that requests audio clips to play
  logo.gif              - Dallas Semiconductor/Maxim logo
  micro4.jpg            - Marketing poo
  multiwaves.tbin       - wav files converted to raw 11KHz samples with table information, store in tbin format
  PlayPostedAudio.java  - Recieves post events from the HTTP server, plays the requested audio
  server.tini           - HTTP server binary executable
  TBINOutputStream.java - TOOL used for converting wav file data to TBIN format
  Wav2TBIN.java         - TOOL used for converting wav file data to TBIN format
  WAVFile.java          - TOOL used for converting wav file data to TBIN format
  setup.jpg             - wiring diagram from DAC board to TINI
