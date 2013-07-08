import parse


def _region_sort_rule(s):
	reglist = [i + str(j) for j in range(1,5) for i in ["FR", "CDR"]]
	return reglist.index(s)


def _parse_header_helper(arg, header):
	try:
		ig_arg = parse.search("%s={:w}" % arg, header).fixed[0]
	except (AttributeError, IndexError):
		ig_arg = "Unknown"
	return ig_arg


class FIGA(object):
	ig_name = "Unknown"
	ig_type = "Unknown"
	ig_alpha = "Unknown"
	ig_specie = "Unknown"
	ig_seq = ""
	ig_regions = {}
	ig_additional = {}

	def set_name(self, newname):
		self.ig_name = newname

	def get_name(self):
		return self.ig_name

	def set_type(self, newtype):
		self.ig_type = newtype

	def get_type(self):
		return self.ig_type

	def set_alpha(self, newalpha):
		self.ig_alpha = newalpha

	def get_alpha(self):
		return self.ig_alpha

	def set_specie(self, newspecie):
		self.ig_specie = newspecie

	def get_specie(self):
		return self.ig_specie

	def set_seq(self, newseq):
		self.ig_seq = newseq

	def get_seq(self):
		return self.ig_seq

	def set_region(self, regionname, region):
		self.ig_regions[regionname] = region

	def get_region(self, regionname):
		return self.ig_regions[regionname]

	def set_additional(self, additionalname, value):
		self.ig_additional[additionalname] = value

	def get_additional(self, additionalname):
		return self.ig_additional[additionalname]

	def dumps(self):
		sorted_regions = sorted(self.ig_regions.items(), key=lambda x: _region_sort_rule(x[0]))
		result = [">%s@(type=%s, alpha=%s, specie=%s)" % (self.ig_name, self.ig_type, 
														  self.ig_alpha, self.ig_specie)]
		for region, value in sorted_regions:
			result.append(";@%s: %s" % (region, value))
		for additional, value in self.ig_additional.items():
			result.append(";+%s: %s" % (additional, value))
		result.append(self.ig_seq)
		return "\n".join(result)

	def parse_header(self, header):
		header = "".join(header.split())
		try:
			self.ig_name = parse.search(">{:S}@", header).fixed[0]
		except (AttributeError, IndexError):
			self.ig_name = "Unknown"
		self.ig_type = _parse_header_helper("type", header)
		self.ig_alpha = _parse_header_helper("alpha", header)
		self.ig_specie = _parse_header_helper("specie", header)
		
	def parse_seq(self, seq):
		self.ig_seq = seq

	def parse_region(self, region):
		region = "".join(region.split())
		try:
			name = parse.search(";@{:w}:", region).fixed[0]
			reg = parse.search(":({:d},{:d})", region).fixed
			self.ig_regions[name] = reg
		except (AttributeError, IndexError):
			pass

	def parse_additional(self, additional):
		additional = "".join(additional.split())
		try:
			name = parse.search(";+{:w}:", additional).fixed[0]
			add = parse.search(":{:w}", additional).fixed[0]
			self.ig_additional[name] = add
		except (AttributeError, IndexError):
			pass

	def parse(self, string):
		lines = string.splitlines(True)
		header = lines[0]
		self.parse_header(header)
		for line in lines[1:]:
			if line.startswith(";@"):
				self.parse_region(line)
			elif line.startswith(";+"):
				self.parse_additional(line)
			else:
				self.parse_seq(line)


def test_generate_figa():
	a = FIGA()
	a.set_name("test-figa")
	a.set_type("VH")
	a.set_specie("llama")
	a.set_alpha("amino")
	a.set_seq("EVQLVQSGAEVKKPGESLKISCKGSGYSFTTYWLGWVRQMPGKGLDWIGIMSPVDSDIRYSPSFQGQVTMSV"
			 "DKSITTAYLQWNSLKASDTAMYYCARRRPGQGYFDFWGQGTLVTVSSASTKGPSVFPLA")
	a.set_region("FR1", (0, 29))
	a.set_region("CDR1", (30, 34))
	a.set_region("FR2", (35, 48))
	a.set_region("CDR2", (49, 65))
	a.set_region("FR3", (66, 95))
	a.set_region("CDR3", (96, 107))
	a.set_region("FR4", (108, len(a.get_seq()) - 1))
	return a.dumps()


def test_load_figa(s):
	a = FIGA()
	a.parse(s)
	return a


def test():
	s = test_generate_figa()
	t = test_load_figa(s).dumps()
	return s == t