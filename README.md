# SPCM 0.1.0-alpha

This is an experimental prototype for the research about Sparsifying
Parity-Check Matrices ([paper]). We include the propotype that was
implemented for testing.

## Table of contents

- [Getting Started]
   - [Prerequisites]
   - [Installing]
   - [Running]
- [Contributing]
- [Versioning]
- [Authors]
- [License]
- [Acknowledgments]

## Getting Started

To get a copy of this software download or clone the GitHub repository.

Download:

```
wget https://github.com/LuisRusso-INESC-ID/SPCM.git
```

Clone:

```
git clone git@github.com:LuisRusso-INESC-ID/SPCM.git
```

### Prerequisites

This package was tested with [Debian], it will most likely work with other
Linux distributions and UNIX variants. Some version of the following
components must exist in the system.

* C compiler, [gcc] or [clang]
* [GNU Make]

### Installing

The makefile provides several compilation options, for debugging, profiling
etc.

To obtain a simple testing binary simply invoque the makefile in the `src`
directory.

```
cd src
make
```

If all went well this should produce a binary named `./project`. This is a
simple binary that uses simulated annealing to sparsify a parity check
matrix. The input matrix should be given in [alist] format. The command
`SYNOPSIS` is the following:

```
SYNOPSIS
./project <input.alist> <output.txt> [<steps> [<iter> [<hotPer> [<hotPr> [<coldPer> [<coldPr>]]]]]]

DESCRIPTION

 The <input.alist> is the input parity check matrix in [alist] format. If
 this string is exactly `stdin` then standard input is used instead. The
 <output.txt> is the name of the file to store the output matrix.

 The remaining arguments are numbers and are optional. The value of <steps>
 is the number of temperatures to consider between the hot and the cold
 temperatures, the default is `100`. The value of <iter> indicates how many
 iterations to execute at a fixed temperature, the default is `100`. The
 <hotPer> value indicates the hit percentage to consider for the hot
 temperature, i.e., what is the percentage of the initial number of non
 zero entries that we consider as an increase. The default is `0.05`, which
 corresponds to 5%. The value <hotPr> is the probability of accepting the
 <hotPer> increase, the default is `0.05` which corresponds to 5%.  The
 <coldPer> value indicates the hit percentage to consider for the cold
 temperature. The default is `0.05`. The value <coldPr> is the probability
 of accepting the <coldPer> increase, the default is `0.01`.
```

Invoquing the binary without the mandatory arguments displays this information.

```
./project
```

We include a sample [BCH] code, in [alist] format, to test the binary. This
file was obtained from [Kaiserslautern University]. You can test the binary
by executing

```
./project ../BCH_63_30_6_strip.alist min.txt
```

On a sample ran we obtained the following first and last lines.

```
# Cost  MinCost Total_Time      Total_Ops       Total_Iterations        Dirty   Temperature
       594             594      3.200000e-05                       0                       0        33  1.051496e+00    
...
       454             454      2.487100e-02                 1331631                    9801         0  6.869613e-01    
```

This format is designed to send to [gnuplot] to produce the plots in the
article. The first line contains the column headers: `Cost` represents the
number of non-zero entries in the current matrix; `MinCost` the minimum
such value obtained from the start of the execution; `Total_Time` is the
time in seconds; `Total_Ops` counts the number of sums of entries that
where executed; `Total_Iterations` is the total amount of iterations;
`Dirty` counts the number of dirty rows in the current configuration;
`Temperature` is the temperature of the current iteration.

You can check the generated matrix in `min.txt`. In our example we obtained
a file that begins as follows:

```
454
33 63
0 1 1 0 0 0 1 0 0 0 1 0 0 0 0 0 0 0 0 0 1 0 1 0 1 1 0 0 0 0 0 0 0 1 0 1 1 1 1 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
...
```

The first number in the first line is the number of non-zero entries. The
second line contains the number of rows followed by the number of
columns. The remaining lines show the matrix. Running the algorithm longer
we can obtain better results. For example you can execute the following
command:

```
./project ../BCH_63_30_6_strip.alist min.txt 10000 100
```

We did a sample execution and obtained a better configuration with only
`420` non zero entries. The underlying algorithm and extensive experimental
results are shown in the [paper].

## Contributing

If you found this project useful please share it and the [paper], also you
can create an [issue] with comments and suggestions, or email me to
[lmsrusso@gmail.com]

## Versioning

We use [SemVer] for versioning. For the versions available, see the [tags]
on this repository.

## Authors

* **Luís M. S. Russo** - *Initial work* - [LuisRusso]

See also the list of [contributors] who participated in this project.

## License

This project is licensed under the BSD 2-Clause "Simplified" License - see
the [LICENSE file] for details

## Acknowledgments

The work reported in this article was supported by national funds through
Fundação para a Ciência e Tecnologia ([FCT]) with reference
UID/CEC/50021/2019. This work was funded by European Union's Horizon 2020
research and innovation programme under the Marie Sklodowska-Curie Actions
grant agreement No 690941, ([BIRDS]). This work was supported by DAAD-CRUP
Luso-German bilateral cooperation under the 2017-2018 research project
MONO-EMC (Multi-Objective Network Optimization for Engineering and
Management Support). This work was supported by the DFG (project-ID:
RU1524/2-3). Jos{\'e} Rui Figueira also acknowledges the support from the
[FCT] grant SFRH/BSAB/139892/2018 under POCH Program.

* Thanks to [PurpleBooth] for the [README-Template].
* The [grip] tool by [Joe Esposito] was very handy for producing this file.

[Getting Started]: #getting-started
[Prerequisites]: #prerequisites
[Installing]: #installing
[Running]: #running
[Contributing]: #contributing
[Versioning]: #versioning
[Authors]: #authors
[License]: #license
[Acknowledgments]: #acknowledgments

[paper]: https://arxiv.org/
[Debian]: https://www.debian.org/
[gcc]: https://gcc.gnu.org/
[clang]: https://clang.llvm.org/
[GNU Make]: https://www.gnu.org/software/make/
[alist]: http://www.inference.org.uk/mackay/codes/alist.html
[BCH]: https://en.wikipedia.org/wiki/BCH_code
[Kaiserslautern University]: https://www.uni-kl.de/channel-codes/channel-codes-database/bch-and-hamming/
[gnuplot]: http://www.gnuplot.info/
[issue]: ../../issues
[lmsrusso@gmail.com]: mailto:lmsrusso@gmail.com
[SemVer]: http://semver.org/
[tags]: ../../tags
[LuisRusso]: https://github.com/LuisRusso
[contributors]: ../../contributors
[LICENSE file]: ./LICENSE
[FCT]: https://www.fct.pt/
[NGPHYLO]: https://thor.inesc-id.pt/ngphylo/
[BIRDS]: http://www.birdsproject.eu/
[README-Template]: https://gist.github.com/PurpleBooth/109311bb0361f32d87a2
[PurpleBooth]: https://gist.github.com/PurpleBooth
[grip]: https://github.com/joeyespo/grip
[Joe Esposito]: https://github.com/joeyespo
