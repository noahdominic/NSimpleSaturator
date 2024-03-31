# NSimpleSaturator

NSimpleSaturator is a basic saturator VST plugin designed for audio processing.

## Installation

You can download the x64 VST3 file from the [Releases page](https://github.com/noahdominic/NSimpleSaturator/releases).  Installation instructions vary depending on your digital audio workstation&nbsp;(DAW).  Generally, you should place the VST3 file in `C:\Program Files\Common Files\VST3` as most DAWs search for plugins there.  In any case, please refer to your DAW's documentation for specific instructions.

## Usage

NSimpleSaturator features the following parameters:

- **Pre-Gain**: Adjusts the input gain level before saturation.
- **Saturation**: Sets the saturation level of the audio wave.
- **Post-gain**: Adjusts the output gain level after clipping.
- **Mix**: Controls the blend between the dry and wet signals.

Simply adjust these parameters according to your preferences to shape the clipped audio signal.

## How clipping works

The algorithm for NSimpleSaturator uses a hyperbolic tangent function $\tanh$ to introduce some nonlinearity to the audio signal for some positive saturation level $k$. The equation 

$$ h(x) = \frac{\tanh(xk)}{k} $$

where $k=$ `mSaturation * UPPER_LIMIT` behaves such that as $k$ approaches $0$, $h(x)$ approaches clean, unprocessed signal $x$ and that as $k$ approaches infinity, $h(x)$ approaches a stepwise function, i.e. a clipped signal which is a maximally processed signal.  In code, $k$ has a limit of 10, mostly because 10 has sufficient-sounding saturation level in my opinion.  Going above 10 would provide too harsh a sound, a sound better fit for NSimpleClipper.

Food for thought: What other nonlinear functions could be used instead of $\tanh$?  Maybe techniques from machine learning could provide some insight here.

## Compatibility

NSimpleSaturator has been tested on 64-bit Windows 11 with PreSonus Studio One and Ableton Live.  While compatibility is not guaranteed, this plugin should be compatible with most DAWs.  Currently, I do not have the resources to test this on macOS.

## Licence

This plugin is distributed under the GPLv3 licence.  See the [LICENSE](https://github.com/noahdominic/NSimpleSaturator/LICENSE.txt) file for details.

## Contact

For support, bug reports, or feature requests, please [send me an email](mailto:noah@noahdominic.com).
