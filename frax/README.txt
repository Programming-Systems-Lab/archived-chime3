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

Adding new Protocols:

        If you wish to add a new protocol you need to mention it in the
        file called protocol.conf. The syntax is the same as entering a
        plug:
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



