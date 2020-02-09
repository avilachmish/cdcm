# cdcm_client_ruby

the cdcm client can be used to test the cdcm functionality.
the client implements the cdcm protocol and the requests are sent using ZMQ sockets.
this client implements the MajorDomo design pattern.

the testRunner reads xml that holds the asset details, the set of actions to run on the asset and verification details for each action.
from the xml the testRunner prepare a structure that holds all of these details, the 'sessions', load it into the  MajorDomoClient 
run options:
* single client 
'ruby mdclient2.rb' 
