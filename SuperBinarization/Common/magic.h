#ifndef MAGIC_H
#define MAGIC_H

#define pointer(className, variable) std::shared_ptr<className> \
                                variable = std::make_shared<className>();

#endif // MAGIC_H
