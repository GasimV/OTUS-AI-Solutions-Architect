# ДЗ-8: Автоматизация поставки AI-сервиса

В репозитории собраны артефакты для домашнего задания по IaC, CI/CD и MLOps:

- `terraform/` - Terraform-конфигурация базовой инфраструктуры в AWS.
- `diagrams/ci_cd_mlops_pipeline.puml` - схема объединенного CI/CD + MLOps конвейера в PlantUML.
- `release_strategy.md` - описание стратегии релиза Canary Deployment и правил отката.

## Что автоматизируется

- Поднятие сетевой инфраструктуры, EKS-кластера, Docker Registry и S3-хранилищ.
- Сборка и проверка приложения после коммита.
- Доставка Docker-образа и версии модели в staging и production.
- Переобучение модели по событию обновления датасета.
- Регистрация модели в Model Registry и автозапуск GitOps-деплоя после approval.

## Логика решения

1. Разработчик пушит код в репозиторий.
2. CI запускает сборку контейнера, unit tests и публикацию образа.
3. Обновление датасета в S3 запускает Airflow DAG, который переобучает модель.
4. После валидации модель регистрируется в Model Registry и попадает в deployment manifest.
5. GitOps-инструмент доставляет связку `image tag + model version` в staging, затем в production через canary rollout.

## Примечание по Terraform

Для реального применения нужно переопределить имена S3 bucket'ов в `terraform/terraform.tfvars.example`, потому что имена bucket'ов в AWS глобально уникальны.
