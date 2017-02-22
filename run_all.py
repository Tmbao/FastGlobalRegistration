import sys
import string
from os import listdir, system
from os.path import isfile, join, dirname, abspath


CUR_PATH  = dirname(abspath(__file__))
FGR_EXEC  = CUR_PATH + '/build/FastGlobalRegistration/FastGlobalRegistration'
EXTR_EXEC   = CUR_PATH + '/extractor/build/extractor'
MESH_FOLDER = CUR_PATH + '/dataset/meshes'
FEAT_FOLDER = CUR_PATH + '/dataset/features'


def main():
  feature_files = []
  for mesh_file in listdir(MESH_FOLDER):
    if isfile(join(MESH_FOLDER, mesh_file)) and join(MESH_FOLDER, mesh_file).endswith('.obj'):
      input_file = join(MESH_FOLDER, mesh_file)
      output_file = string.replace(join(FEAT_FOLDER, mesh_file), '.obj', '.bin')
      system('%s %s %s' % (EXTR_EXEC, input_file, output_file))

      feature_files.append(output_file)

  for i in xrange(len(feature_files)):
    for j in xrange(len(feature_files)):
      if (i <= j):
        temp_file = 'output.txt'
        system('%s %s %s %s' % (FGR_EXEC, feature_files[i], feature_files[j], temp_file))
        print('%s %s\n' % (feature_files[i], feature_files[j]))
        system('cat %s' % temp_file)


if __name__ == '__main__':
  main()
