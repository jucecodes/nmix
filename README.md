# nmix

nmix is a [Juce](https://www.juce.com) powered prototype exploring the  functionality of coordinate oriented audio mixing.

## Cloning

	git clone --recursive https://github.com/lassandroan/nmix.git

When cloning this repository, make sure you include the ```--recursive``` flag to include the juce submodule.

## Building

[Xcode](http://appstore.com/mac/xcode) and the [Xcode Command Line Tools](http://railsapps.github.io/xcode-command-line-tools.html) are required on macOS.

First, build the Projucer app using the relevant IDE project from ```juce/extras/Projucer/Builds/```

Use the newly built Projucer to load and save ```nmix.jucer``` to generate the IDE projects for building.

The process (including compilation) has been automated for macOS via the ```build``` script:

	./build [-t iOS | MacOSX | All]

## License

This project is licensed under GPLv3 - see the [LICENSE](LICENSE) file for details

## Acknowledgments

This project is based on developments done by Juan Pablo Carrascal and Sergi Jordà in their article [Multitouch Interface for Audio Mixing](http://www.nime2011.org/proceedings/papers/B19-Carrascal.pdf).
