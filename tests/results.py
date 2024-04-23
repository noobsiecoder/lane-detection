import timeit


def precision(true_positives, false_positives):
    '''
    Precision measures the proportion of true positive predictions among all positive predictions made by the model
    '''
    return true_positives / (true_positives + false_positives)


def recall(true_positives, false_negatives):
    '''
    Recall measures the proportion of true positive predictions among all actual positive instances in the dataset
    '''
    return true_positives / (true_positives + false_negatives)


def f1_score(precision, recall):
    '''
    It is the harmonic mean of precision and recall
    Ranges between 0 and 1, with higher values indicating better model performance
    '''
    return (2 * precision * recall) / (precision + recall)


data = [{'name': 'IMG_0001', 'frames': 108, 'true_positives': 98,
         'false_positives': 50, 'false_negatives': 7},
        {'name': 'IMG_0002', 'frames': 75, 'true_positives': 73,
         'false_positives': 20, 'false_negatives': 2},
        {'name': 'IMG_00011', 'frames': 233, 'true_positives': 209,
         'false_positives': 96, 'false_negatives': 24},
        {'name': 'IMG_00015', 'frames': 233, 'true_positives': 230,
         'false_positives': 12, 'false_negatives': 68}]


if __name__ == "__main__":
    precisions = []
    sensitivities = []
    scores = []

    for field in data:
        prec = precision(
            field['true_positives'], field['false_positives'])
        sensitivity = recall(field['true_positives'], field['false_negatives'])
        score = f1_score(prec, sensitivity)
        precisions.append(prec)
        sensitivities.append(sensitivity)
        scores.append(score)

    print(f'P({((sum(precisions) / len(precisions)) * 100):.4f}), S({((sum(sensitivities) / len(sensitivities)) * 100):.4f}), F1({(sum(scores) / len(scores)):.4f})')
