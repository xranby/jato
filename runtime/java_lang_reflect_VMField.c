#include "runtime/java_lang_reflect_VMField.h"

#include "vm/annotation.h"
#include "vm/reflection.h"
#include "vm/errors.h"
#include "vm/class.h"

#include <stddef.h>
#include <stdlib.h>

jobject java_lang_reflect_VMField_getAnnotation(jobject this, jobject annotation)
{
	struct vm_class *annotation_type;
	struct vm_field *vmf;
	unsigned int i;

	annotation_type = vm_object_to_vm_class(annotation);

	vmf = vm_object_to_vm_field(this);
	if (!vmf)
		return rethrow_exception();

	for (i = 0; i < vmf->nr_annotations; i++) {
		struct vm_annotation *vma = vmf->annotations[i];
		struct vm_object *annotation;

		if (vm_annotation_get_type(vma) != annotation_type)
			continue;

		annotation = vm_annotation_to_object(vma);
		if (!annotation)
			return rethrow_exception();

		return annotation;
	}

	return NULL;
}