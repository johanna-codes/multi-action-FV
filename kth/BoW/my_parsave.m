function my_parsave(fname, hist)


hdf5write(fname, '/dataset1', hist);

  %To save in Matlab with name myfile.h5, the matrix generated with uint8(magic(5))
  %hdf5write('myfile.h5', '/dataset1', uint8(magic(5)));