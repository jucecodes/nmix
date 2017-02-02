# nmix

nmix is a [Juce](https://www.juce.com) powered prototype exploring the  functionality of coordinate oriented audio mixing.

## Setup

### Cloning


	git clone --recursive https://github.com/lassandroan/nmix.git

When cloning this repository, make sure you include the ```--recursive``` flag to include the juce submodule.

### Building

Because juce has been included as a submodule, there is no need to install or configure juce itself. However, the [Projucer](juce/extras/Projucer) must be built first in order to load the nmix jucer project. The Projucer includes [IDE projects](juce/extras/Projucer/Builds) already, so simply load the project in your IDE and hit build.

After the Projucer has been built, it can be used to load the nmix jucer project and generate an IDE project for nmix.

## License

This project is licensed under GPLv3 - see the [LICENSE](LICENSE) file for details

## Acknowledgments

This project is based on developments done by Juan Pablo Carrascal and Sergi Jordà in their article [Multitouch Interface for Audio Mixing](http://www.nime2011.org/proceedings/papers/B19-Carrascal.pdf).
