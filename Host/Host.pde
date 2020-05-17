import hypermedia.net.*;
UDP udp;
String dataKey = "yhmJCRBs";
void setup() {
  udp = new UDP( this, 3333 );
  udp.listen( true );
}
void draw() {;}
void receive( byte[] data, String ip, int port ) {
  data = subset(data, 0, data.length);
  String message = new String( data );
  StringBuilder messageBuilder = new StringBuilder( message );
  for(int i = 0; i < message.length(); i++)
  {
    messageBuilder.setCharAt(i, (char)(message.charAt(i) ^ dataKey.charAt(i % 8)));
  }
  message = messageBuilder.toString();
  println( "receive: \""+message+"\" from "+ip+" on port "+port );
}
