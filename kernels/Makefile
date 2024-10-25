# 顶级 Makefile  
  
# 定义变量  
SUBDIRS=$(shell find . -maxdepth 1 -mindepth 1 -type d)  
  
# 默认目标（可选）  
all:  
	@echo "Running default target (you can customize this)"  
  
# 递归清理所有子目录的目标  
cleanall:  
	@for dir in $(SUBDIRS); do \
		if [ -f "$$dir/Makefile" ]; then \
				(cd "$$dir" && $(MAKE) clean) || exit 1; \
		fi \
	done
  
# 伪目标，防止与文件名冲突  
.PHONY: cleanall
