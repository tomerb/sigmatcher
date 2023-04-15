"""
Generate two datasets that are used for validating the correctness of the
sigmatch library.

a dataset is a collection of files, each in different size (random range between
the given constraints).
The content of the files is randomly generated for each file, and is a list of
random lower-case ascii letters.

Each dataset is configured differently, as defined in the Dataset class.
"""

from random import choice, randint
from string import ascii_letters as alphabet
from dataclasses import dataclass
from uuid import uuid4
from time import time
import os

@dataclass
class Dataset:
    name: str
    num_files: int
    file_min_size_kb: int
    file_max_size_kb: int
    output_dir: str

malicious_config = Dataset("malicious", 500, 8, 5000, "./malicious_dataset")
benign_config = Dataset("benign", 2000, 100, 10000, "./benign_dataset")

def gen_random_file(size_min_kb, size_max_kb, output_dir):
    outfile = output_dir + "/" + str(uuid4()) + ".dat"
    with open(outfile, 'w') as f:
        for i in range(randint(size_min_kb*1024, size_max_kb*1024)):
            f.write(choice(alphabet))

def gen_multiple_files(number_of_files, size_min_kb, size_max_kb, output_dir):
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    for i in range(number_of_files):
        gen_random_file(size_min_kb, size_max_kb, output_dir)

def gen_dataset(dataset):
    print("Generating " + dataset.name + " dataset with " +
          str(dataset.num_files) + " files ("             +
          str(dataset.file_min_size_kb) + "KB-"           +
          str(dataset.file_max_size_kb) + "KB)...")
    gen_multiple_files(dataset.num_files,
                       dataset.file_min_size_kb,
                       dataset.file_max_size_kb,
                       dataset.output_dir)

start = time()
gen_dataset(malicious_config)
gen_dataset(benign_config)
end = time()

print("Done! (took " + str(round(end-start)) + " seconds)")

os.system('du -h')
