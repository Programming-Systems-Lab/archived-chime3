Programming Systems Lab
Columbia Hypermedia Immersive Environment (CHIME) v3.0 README
Copyright (c) 2001 The Trustees of Columbia University in the City of
New York.  All Rights Reserved.
----------------------------------------

* Introduction

CHIME consists of 8 modules - viz. FRAX, plugs, Dataserver, VEM, ThemeManager, Client, proxy and the 2D client. Here we are just showing what FRAX, the plugs and the Dataserver look like.

Note that CHIME is in a prerelease state, and is still active research. This means that while the functionality mentioned in this document has been implemented, there is no guarantee of error-free operation. Additionally, modules in this system are subject to significant change in the future; current API's are not final-form.

* Description

FRAX - File Recognize and XMLify
This is the module that goes out and contacts the exteral data source using the inputted location and the specified protocol to extract metadata from the remote source.

   Adding new Protocols:

        If you wish to add a new protocol you need to mention it in the
        file called protocol.conf. The format to be used should look like:
        <protocol> <class responsible for protocol>
        HTML psl.chime.frax.protocols.http

        Protocols should be put in the protocols/ directory and they
        must be part of the package psl.chime.frax.protocols.<ProtocolName>
        All protocols should extend the psl.chime.frax.FRAXProtocol which
        provides helper methods. See the comments of the source file.
        Generally speaking a protocol needs to implement createParams which
        is defined in FRAXProtocol and goPlug. The method createParams
        generates the argument which needs to be passed to goPlug. (the
        Object[]). goPlug passes the control to the plug decider which
        calls on the appropriate plug.


   Adding Special Protocols:

        If you wish to add special protocols you can modify PlugStarter.java
        where you will find a method called getType. This method was created
        especially for protocols that need to use data sources which don't
        implement extensions.


The Plugs are called specifically by FRAX depending on the file type, each plug is specifically designed to extract metadata from specific filetypes.

   Adding new plugs:

        If you wish to add new plugs then you need to mention them in the
        file called plug.conf. The format of the entry should correspond
        to the following:
        <extension> <class responsible for extension>
        HTML pls.chime.frax.plugs.html

        Plugs should be put in the plugs/ directory
        and they should all be in the package psl.chime.frax.plugs.<PlugName>.
        All plugs must extend psl.chime.frax.plugs.FRAXPlug which will force
        the plug writer to implement all the procedures which are used in frax

The information from FRAX/plugs is posted onto the Siena server in the form of SmartEvents. For this demo, the server will start up locally on your machine and so will the subscriptions so that you can monitor them.

The DataServer accepts these SmartEvents and parses through them to grab the metadata and post it cleanly into a database form. This part has not been integrated fully yet, but should be done in about a week.


* Distribution/Prerequisites

CHIME is currently distributed in source.  In order to use CHIME, you must have several supporting libraries:

- SUN JDK 1.3 (http://java.sun.com/)
- Siena 1.1.3 (http://www.cs.colorado.edu/~carzanig/siena)
- Apache Xerces XML parser (http://xml.apache.org/)

If you installed the complete installation available from our website, then both Siena and the XML parser should have been preinstalled for you.

Once source has been obtained, ensure that the CLASSPATH includes the aforementioned libraries as well as the PARENT of the psl directory. Your classpath should be as follows

SET ROOT=C:\pslroot
SET FRAX=%ROOT%\psl\chime\frax
SET JARS=%ROOT%\jars\siena-1.1.2.jar;%ROOT%\jars\HTTPClient.jar
SET CLASSPATH=%ROOT%;%JARS%

We have put this in a file called setenv-vars.bat and if you run it, it will automatically add this to your classpath. Please remember that these are not permanent additions to your classpath and will need to be added after you reboot. You must add this to your classpath/run the file before running your own test cases on the command line. To run our test cases, you do not need this.

* Configuration

Although CHIME is meant to be very dynamic, most of the work here has been hard-coded and will be changed in the next version so that it can be changed at runtime.

* Execution

Note that CHIME is not intended for standalone operation.  However, a tester program is provided so that you can experiment with its functionality and begin to be accustomed to the CHIME prerelease API.
 Usage :
	java psl.chime.frax.FRAX <Protocol> <Object to retrieve>

	Protocol can be either of the following:
		HTTP - web requests
		LFS  - local file system requests
		BSCW - BSCW requests

	Object to retrieve:
		anything that the protocol understands.

* Documentation

Published documentation is not yet available for CHIME, as the API is currently under heavy fluctuation.  However, CHIME source is well-commented, and follows Javadoc standards, so if basic
documentation is desired javadoc should be run on the supplied modules.

* Known Problems/Bugs

Due to the early research state of this software, there are a number of known bugs and issues.  We are in the process of compiling these, and will be placing them on our website in the near future.  In the meantime, see the email address below for support on CHIME.

* Contact

PSL can be reached on the Web at http://www.psl.cs.columbia.edu.  For technical support with this product (limited, as it is a prerelease version), email marvelus@cs.columbia.edu.