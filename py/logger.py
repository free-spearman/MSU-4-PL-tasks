from collections import deque

from time import time_ns
from resource import getrusage, RUSAGE_SELF

from datetime import datetime 



class BaseLogger():
	"""docstring for BaseLogger"""
	def __init__(self, log_name, class_name):
		self.log_name = log_name
		self.class_name = class_name
		self.start_times = deque()
		self.call_stack = deque()
		self.out = open(self.log_name, 'a')
		self.out.write(f"\nBEGIN LOG:{BaseLogger.time_label()}\n\n")
	def time_label():
		"""возвращает временную метку %d-%m-%Y %H:%M:%S """
		label_tm = datetime.now()
		return label_tm.strftime("%d-%m-%Y %H:%M:%S")
	def get_time():
		return time_ns()
	def get_mem():
		return getrusage(RUSAGE_SELF).ru_maxrss 
	def log(self, out_sring, operation):
		""" вывод обычного лога """
		prefix = f"{self.class_name}::{operation}::{BaseLogger.time_label()}\n" 
		if self.out.closed:
			self.out = open(self.log_name, 'a')

		self.out.write(prefix)
		self.out.write(out_sring)
		pass
	def start_log(self, operation):
		self.call_stack.append(operation)
		self.start_times.append(BaseLogger.get_time())
		pass
	def end_log(self):
		end_time = BaseLogger.get_time()
		call = self.call_stack.pop()
		start_time = self.start_times.pop()
		if self.out.closed:
			self.out = open(self.log_name, 'a')
		prefix = f"{self.class_name}::{call}::{BaseLogger.time_label()}\n"
		out_sring = f"время:{(end_time - start_time)/10**9}|память:{BaseLogger.get_mem()}\n"
		self.out.write(prefix)
		self.out.write(out_sring)
		pass
	def end(self):
		close(self.out)