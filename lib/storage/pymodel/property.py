__author__ = 'Aspart'

import json
import os.path

class Property:
    '''
    Store properties of gene class in JSON format. Helps to load and save properties, get and set values of keys.
    '''
    def load(self, path):
        json_data=open(os.path.join(path, '.json'))
        self.data = json.load(json_data)
        json_data.close()


    def save(self, path):
        if not os.path.exists(path):
            os.makedirs(path)
        with open(os.path.join(path, '.json'), 'w') as outfile:
            json.dump(self.data, outfile)


    def getValue(self, name):
        ret = self.data[name]
        return ret


    def setValue(self, name, value):
        self.data[name] = value