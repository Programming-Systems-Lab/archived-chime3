FRAX=~/CHIME/FRAX/FRAX_lib; export FRAX
SIENA=~/CHIME/Siena/; export SIENA;
FRAXJARS=$FRAX/Protocols/HTTP/HTTPClient.jar; export JARS
SIENAJARS=$SIENA/siena-1.1.1-jvm1.3.jar; export SIENAJARS
CLASSPATH=$CLASSPATH:.:$FRAX:$FRAXJARS:$SIENAJARS:$SIENA; export CLASSPATH

rm $FRAX/*.class
rm $FRAX/Plugs/*.class
rm $FRAX/Plugs/IMG/*.class
rm $FRAX/Plugs/HTML/*.class
rm $FRAX/Plugs/DIR/*.class
rm $FRAX/Plugs/XML/*.class

rm $FRAX/Protocols/*.class
rm $FRAX/Protocols/HTTP/*.class
rm $FRAX/Protocols/LFS/*.class
rm $FRAX/Protocols/BSCW/*.class
rm $FRAX/FRAXExceptions/*.class

javac $SIENA/*.java
javac $FRAX/*.java
javac $FRAX/Plugs/*.java
javac $FRAX/Plugs/IMG/*.java
javac $FRAX/Plugs/HTML/*.java
javac $FRAX/Plugs/DIR/*.java
javac $FRAX/Plugs/XML/*.java

javac $FRAX/Protocols/*.java
javac $FRAX/Protocols/HTTP/*.java
javac $FRAX/Protocols/LFS/*.java
javac $FRAX/Protocols/BSCW/*.java
javac $FRAX/FRAXExceptions/*.java


