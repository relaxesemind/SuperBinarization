#ifndef MAGIC_H
#define MAGIC_H

#define pointer_magic(className, variable) std::shared_ptr<className> \
                                variable = std::make_shared<className>();

#define for_magic(_iter_, _container_) \
    for (auto _iter_ = _container_.begin(); _iter_ != _container_.end(); ++_iter_)

template<class CheckType = int, class InstanceType>
bool magic_isInstanceOf(const InstanceType &Instance)
{
  return dynamic_cast<CheckType *>(&Instance) != 0;
}

#endif // MAGIC_H
